#pragma once

#include <coroutine>
#include <cstddef>
#include <cstdint>
#include <format>
#include <functional>
#include <stdexcept>
#include <string>
#include <string_view>
#include <thread>
#include "log.hpp"

#ifdef _WIN32
    #ifndef WIN32_LEAN_AND_MEAN
        #define WIN32_LEAN_AND_MEAN
    #endif

    #include <windows.h>

    #include <WinSock2.h>
    #include <ws2tcpip.h>

    #pragma comment(lib, "Ws2_32.lib")
#endif

namespace http {

static constexpr size_t BUFFER_SIZE = 4096;

struct ClientContext {
    OVERLAPPED overlapped;     // Overlapped 구조체
    WSABUF wsabuf;             // WSA 버퍼
    char buffer[BUFFER_SIZE];  // 데이터 버퍼
    SOCKET socket;             // 클라이언트 소켓
};

struct SocketConfig {
    uint16_t port = 3000;
    size_t max_threads = 8;
};

#ifdef _WIN32
class Socket {
public:
    struct Listener {
        std::function<void()> on_connect;
        std::function<void()> on_disconnect;
        std::function<void(std::string_view)> on_receive;
    };

public:
    // virtual void on_connect(SOCKET client_socket);
    // virtual void on_disconnect(SOCKET client_socket);
    // virtual void on_receive(SOCKET client_socket, std::string_view data);

    Socket(SocketConfig config = {}) noexcept : config(config) {
        LOG_TRACE("http::Socket(SocketConfig config)");
    }
    Socket(SocketConfig&& config) noexcept : config(std::move(config)) {
        LOG_TRACE("http::Socket(SocketConfig&& config)");
    }
    Socket(Socket&) = delete;
    Socket(Socket&& other) noexcept : config(std::move(other.config)) {
        LOG_TRACE("http::Socket(Socket&& other)");
    };

    Socket& operator=(Socket&) = delete;
    Socket& operator=(Socket&& other) {
        LOG_TRACE("http::Socket::operator(Socket&& other)");
        self.config = std::move(other.config);
        return self;
    }

    ~Socket() {
        LOG_TRACE("http::~Socket()");
        if (self.ready) {
            self.terminate();
        }
    }

    void init() {
        LOG_TRACE("http::Socket::init()");

        if (self.ready) {
            return;
        }

        WSADATA wsa_data;
        int32_t startup_result = WSAStartup(MAKEWORD(2, 2), &wsa_data);

        if (startup_result != 0) {
            throw std::runtime_error(std::format(
                "Cannot initialize winsock: {}", WSAGetLastError()));
        }

        struct addrinfo *addr_info = nullptr, *ptr = nullptr, hints;

        ZeroMemory(&hints, sizeof(hints));
        hints.ai_family = AF_INET;
        hints.ai_socktype = SOCK_STREAM;
        hints.ai_protocol = IPPROTO_TCP;
        hints.ai_flags = AI_PASSIVE;

        int32_t addrinfo_result = getaddrinfo(nullptr,
            (std::to_string(self.config.port)).c_str(), &hints, &addr_info);

        if (addrinfo_result != 0) {
            WSACleanup();
            throw std::runtime_error(std::format(
                "Cannot get address information: {}", WSAGetLastError()));
        }

        self.socket = WSASocketW(hints.ai_family, hints.ai_socktype,
            hints.ai_protocol, nullptr, 0, WSA_FLAG_OVERLAPPED);

        if (self.socket == INVALID_SOCKET) {
            freeaddrinfo(addr_info);
            WSACleanup();
            throw std::runtime_error(
                std::format("Cannot create socket: {}", WSAGetLastError()));
        }

        int32_t bind_result = ::bind(self.socket, addr_info->ai_addr,
            static_cast<int32_t>(addr_info->ai_addrlen));

        if (bind_result != 0) {
            freeaddrinfo(addr_info);
            closesocket(self.socket);
            WSACleanup();
            throw std::runtime_error(
                std::format("Cannot bind socket: {}", WSAGetLastError()));
        }

        freeaddrinfo(addr_info);

        int32_t listen_result = ::listen(self.socket, SOMAXCONN);

        if (listen_result == SOCKET_ERROR) {
            self.terminate();
            throw std::runtime_error(
                std::format("Socket listen failed: {}", WSAGetLastError()));
        }

        self.iocp = CreateIoCompletionPort(INVALID_HANDLE_VALUE, nullptr, 0, 0);

        if (self.iocp == nullptr) {
            self.terminate();
            throw std::runtime_error(
                std::format("Cannot create IOCP: {}", WSAGetLastError()));
        }

        // if (CreateIoCompletionPort(
        //         reinterpret_cast<HANDLE>(self.socket), iocp, 0, 0) ==
        //         nullptr) {
        //     CloseHandle(iocp);
        //     self.terminate();
        //     throw std::runtime_error(std::format(
        //         "Cannot register socket to IOCP: {}", WSAGetLastError()));
        // }

        self.worker_threads.reserve(self.config.max_threads);
        for (size_t i = 0; i < self.config.max_threads; ++i) {
            // self.worker_threads.emplace_back(&Socket::worker_thread, this);
            self.worker_threads.emplace_back(
                [this]() { this->worker_thread(); });
        }

        self.ready = true;
    }

    void listen() {
        LOG_TRACE("http::Socket::listen()");

        if (!self.ready) {
            std::println("Server is not ready. Initialize server first");
            return;
        }

        while (true) {
            SOCKADDR_IN client_address;
            int32_t address_length = sizeof(client_address);
            SOCKET client_socket = ::accept(self.socket,
                reinterpret_cast<sockaddr*>(&client_address), &address_length);

            if (client_socket == INVALID_SOCKET) {
                std::println(
                    "Failed to accept connection: {}", WSAGetLastError());
                continue;
            }

            ClientContext* client_context = new ClientContext{};
            client_context->socket = client_socket;
            client_context->wsabuf.buf = client_context->buffer;
            client_context->wsabuf.len = BUFFER_SIZE;

            if (CreateIoCompletionPort(reinterpret_cast<HANDLE>(client_socket),
                    self.iocp, reinterpret_cast<ULONG_PTR>(client_context),
                    0) == nullptr) {
                self.terminate();
                throw std::runtime_error(
                    "Failed to associate socket with IOCP");
            }

            DWORD flags = 0;
            int32_t receive_result =
                WSARecv(client_socket, &client_context->wsabuf, 1, nullptr,
                    &flags, &client_context->overlapped, nullptr);

            if (self.listeners.on_connect) {
                self.listeners.on_connect();
            }

            using namespace std::chrono_literals;
            std::this_thread::sleep_for(1ms);
        }
    }

    void on_connect(std::function<void()> func) {
        LOG_TRACE("http::Socket::on_connect()");
        self.listeners.on_connect = func;
    }
    void on_disconnect(std::function<void()> func) {
        LOG_TRACE("http::Socket::on_disconnect()");
        self.listeners.on_disconnect = func;
    }
    void on_receive(std::function<void(std::string_view)> func) {
        LOG_TRACE("http::Socket::on_receive()");
        self.listeners.on_receive = func;
    }

    void terminate() {
        LOG_TRACE("http::Terminate Socket");

        if (!self.ready) {
            return;
        }

        self.ready = false;

        if (self.iocp != nullptr) {
            CloseHandle(self.iocp);
        }

        if (self.socket != SOCKET_ERROR) {
            closesocket(self.socket);
        }

        WSACleanup();
    }

private:
    void worker_thread() {
        LOG_TRACE("http::Socket::worker_thread()");

        while (true) {
            if (!self.ready) {
                break;
            }

            self.worker_thread_loop();

            using namespace std::chrono_literals;
            std::this_thread::sleep_for(1ms);
        }
    }
    void worker_thread_loop() {
        LOG_TRACE("http::Socket::worker_thread_loop()");
        DWORD bytes_transferred;
        ClientContext* client_context;
        OVERLAPPED* overlapped;

        BOOL result = GetQueuedCompletionStatus(self.iocp, &bytes_transferred,
            reinterpret_cast<PULONG_PTR>(&client_context), &overlapped,
            INFINITE);

        if (!result || bytes_transferred == 0) {
            if (client_context != nullptr) {
                if (self.listeners.on_disconnect) {
                    self.listeners.on_disconnect();
                }
                closesocket(client_context->socket);
                delete client_context;
            }

            return;
        }

        if (overlapped == &client_context->overlapped) {
            std::string received_data(
                client_context->buffer, bytes_transferred);

            if (self.listeners.on_receive) {
                self.listeners.on_receive(std::string_view(received_data));
            }

            memset(client_context->buffer, 0, BUFFER_SIZE);
            client_context->wsabuf.buf = client_context->buffer;
            client_context->wsabuf.len = BUFFER_SIZE;
        }

        DWORD flags = 0;

        WSARecv(client_context->socket, &client_context->wsabuf, 1, nullptr,
            &flags, &client_context->overlapped, nullptr);
    }

private:
    Socket& self = *this;

    SocketConfig config;
    SOCKET socket = INVALID_SOCKET;
    std::vector<std::jthread> worker_threads;
    HANDLE iocp;
    Socket::Listener listeners{};
    bool ready = false;
};
#elif __linux__

#else
class Socket {
public:
    Socket() {
        std::println("Not implemented");
    }
}
#endif

}  // namespace http

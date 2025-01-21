#include "socket.hpp"
#include "log.hpp"
#include "response.hpp"

namespace http {

#ifdef _WIN32

Socket::Socket() noexcept : config(SocketConfig{}) {
    LOG_TRACE("http::Socket()");
}
Socket::Socket(SocketConfig config) noexcept : config(config) {
    LOG_TRACE("http::Socket(SocketConfig config)");
}
Socket::Socket(SocketConfig&& config) noexcept : config(std::move(config)) {
    LOG_TRACE("http::Socket(SocketConfig&& config)");
}
Socket::Socket(Socket&& other) noexcept : config(std::move(other.config)) {
    LOG_TRACE("http::Socket(Socket&& other)");
};

Socket& Socket::operator=(Socket&& other) {
    LOG_TRACE("http::Socket::operator(Socket&& other)");
    self.config = std::move(other.config);
    return self;
}

Socket::~Socket() {
    LOG_TRACE("http::~Socket()");
    if (self.ready) {
        self.terminate();
    }
}

void Socket::init() {
    LOG_TRACE("http::Socket::init()");

    if (self.ready) {
        return;
    }

    WSADATA wsa_data;
    int32_t startup_result = WSAStartup(MAKEWORD(2, 2), &wsa_data);

    if (startup_result != 0) {
        throw std::runtime_error(
            std::format("Cannot initialize winsock: {}", WSAGetLastError()));
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
        self.worker_threads.emplace_back([this]() { this->worker_thread(); });
    }

    self.ready = true;
}

void Socket::listen() {
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
            std::println("Failed to accept connection: {}", WSAGetLastError());
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
            throw std::runtime_error("Failed to associate socket with IOCP");
        }

        DWORD flags = 0;
        int32_t receive_result = WSARecv(client_socket, &client_context->wsabuf,
            1, nullptr, &flags, &client_context->overlapped, nullptr);

        if (self.listeners.on_connect) {
            self.listeners.on_connect();
        }

        using namespace std::chrono_literals;
        std::this_thread::sleep_for(1ms);
    }
}

void Socket::send(ClientContext* client_context, std::string_view message) {
    size_t buffer_size = message.size();
    char* send_buffer = new char[buffer_size];
    std::memcpy(send_buffer, message.data(), buffer_size);
    client_context->wsabuf.buf = send_buffer;
    client_context->wsabuf.len = buffer_size;

    DWORD bytes_sent = 0;
    DWORD flags = 0;

    int32_t result = WSASend(client_context->socket, &client_context->wsabuf, 1,
        &bytes_sent, flags, &client_context->overlapped, nullptr);

    if (result == SOCKET_ERROR && WSAGetLastError() != WSA_IO_PENDING) {
        LOG_ERROR("Failed to send data to client: {}", WSAGetLastError());
        closesocket(client_context->socket);
    }

    delete[] send_buffer;
}

void Socket::on_connect(std::function<void()> func) {
    LOG_TRACE("http::Socket::on_connect()");
    self.listeners.on_connect = func;
}
void Socket::on_disconnect(std::function<void()> func) {
    LOG_TRACE("http::Socket::on_disconnect()");
    self.listeners.on_disconnect = func;
}
void Socket::on_receive(
    std::function<std::optional<std::vector<Response>>(std::string_view)>
        func) {
    LOG_TRACE("http::Socket::on_receive()");
    self.listeners.on_receive = func;
}

void Socket::terminate() {
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

void Socket::worker_thread() {
    LOG_TRACE("http::Socket::worker_thread()");

    while (true) {
        if (!self.ready) {
            break;
        }

        DWORD bytes_transferred;
        ClientContext* client_context = nullptr;
        OVERLAPPED* overlapped = nullptr;

        BOOL result = GetQueuedCompletionStatus(self.iocp, &bytes_transferred,
            reinterpret_cast<PULONG_PTR>(&client_context), &overlapped,
            INFINITE);

        if (result == FALSE || bytes_transferred == 0) {
            if (client_context != nullptr) {
                if (self.listeners.on_disconnect) {
                    self.listeners.on_disconnect();
                }
                LOG_ERROR("Client disconnected or error occurred");
                closesocket(client_context->socket);
                delete client_context;
            }
            continue;
        }

        if (overlapped == &client_context->overlapped) {
            std::string received_data(
                client_context->buffer, bytes_transferred);

            LOG_TRACE("Received data: {}", received_data);

            if (self.listeners.on_receive) {
                std::optional<std::vector<Response>> responses =
                    self.listeners.on_receive(std::string_view(received_data));

                if (responses.has_value()) {
                    auto r = responses.value();
                    for (const Response& response : r) {
                        std::string message =
                            Response::response_to_message(response);
                        LOG_TRACE("Sending response: {}", message);
                        self.send(client_context, message);
                    }
                }
            }

            memset(client_context->buffer, 0, BUFFER_SIZE);
            client_context->wsabuf.buf = client_context->buffer;
            client_context->wsabuf.len = BUFFER_SIZE;
            ZeroMemory(&client_context->overlapped, sizeof(OVERLAPPED));
        }

        DWORD flags = 0;

        int32_t recv_result =
            WSARecv(client_context->socket, &client_context->wsabuf, 1, nullptr,
                &flags, &client_context->overlapped, nullptr);

        if (recv_result == SOCKET_ERROR &&
            WSAGetLastError() != WSA_IO_PENDING) {
            LOG_ERROR(
                "Failed to receive data from client: {}", WSAGetLastError());
            closesocket(client_context->socket);
            delete client_context;
        }
    }
}
#else

#endif

}  // namespace http
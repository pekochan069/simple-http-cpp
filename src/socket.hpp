#pragma once

#include <coroutine>
#include <cstddef>
#include <cstdint>
#include <format>
#include <functional>
#include <optional>
#include <stdexcept>
#include <string>
#include <string_view>
#include <thread>

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

class Response;

#ifdef _WIN32
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
class Socket {
public:
    struct Listener {
        std::function<void()> on_connect;
        std::function<void()> on_disconnect;
        std::function<std::optional<std::vector<Response>>(std::string_view)>
            on_receive;
    };

public:
    // virtual void on_connect(SOCKET client_socket);
    // virtual void on_disconnect(SOCKET client_socket);
    // virtual void on_receive(SOCKET client_socket, std::string_view data);

    Socket() noexcept;
    Socket(SocketConfig config) noexcept;
    Socket(SocketConfig&& config) noexcept;
    Socket(Socket&& other) noexcept;
    Socket& operator=(Socket&& other);

    Socket(Socket&) = delete;
    Socket& operator=(Socket&) = delete;

    ~Socket();

    void init();
    void listen();
    void terminate();
    void send(ClientContext* client_context, std::string_view message);

    void on_connect(std::function<void()> func);
    void on_disconnect(std::function<void()> func);
    void on_receive(
        std::function<std::optional<std::vector<Response>>(std::string_view)>
            func);

private:
    void worker_thread();

private:
    Socket& self = *this;

    SocketConfig config;
    SOCKET socket = INVALID_SOCKET;
    std::vector<std::jthread> worker_threads;
    HANDLE iocp;
    Listener listeners{};
    bool ready = false;
};
#elif
class Socket {
public:
    Socket() {
        std::println("Not implemented");
    }
}
#endif

}  // namespace http

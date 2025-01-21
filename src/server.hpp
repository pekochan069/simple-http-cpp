#pragma once
#include "request.hpp"
#include "socket.hpp"

namespace http {

class Response;

struct ServerConfig {
    std::string_view host = "localhost";
    uint16_t port = 3000;
    size_t max_threads = 8;
};

class Server {
public:
    Server();

    Server(ServerConfig server_config);
    Server(Server&& other);

    Server& operator=(Server&& other);
    Server(Server&) = delete;
    Server& operator=(Server&) = delete;

    void listen();
    void on_receive(std::function<std::vector<Response>(Request&&)> func);

    std::string_view get_host() const noexcept;

private:
    Server& self = *this;

    Socket socket;
    ServerConfig config;
};

}  // namespace http
#pragma once

#include "request.hpp"
#include "socket.hpp"
namespace http {

class Server {
public:
    Server() : socket(Socket()) {}

    Server(uint16_t port = 3000, size_t max_threads = 8) {
        SocketConfig config{.port = port, .max_threads = max_threads};
        self.socket = Socket(config);
    }
    Server(SocketConfig config) : socket(Socket(config)) {}
    Server(Server&& other) : socket(std::move(other.socket)) {}

    Server& operator=(Server&& other) {
        self.socket = std::move(other.socket);
        return self;
    }

    Server(Server&) = delete;
    Server& operator=(Server&) = delete;

    void listen() {
        self.socket.init();
        self.socket.listen();
    };

    void on_receive(std::function<void(Request&&)> func) {
        self.socket.on_receive([this, func](std::string_view raw_input) {
            std::optional<Request> request = Request::create(raw_input);
            if (request.has_value()) {
                func(std::move(request.value()));
            } else {
                LOG_ERROR("Invalid request");
            }
        });
    }

private:
    Server& self = *this;

    Socket socket;
};

}  // namespace http
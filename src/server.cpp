#include "server.hpp"
#include "log.hpp"
#include "response.hpp"

namespace http {

Server::Server() : socket(Socket()) {}
Server::Server(ServerConfig server_config) : config(server_config) {
    SocketConfig socket_config{
        .port = server_config.port, .max_threads = server_config.max_threads};
    self.socket = Socket(socket_config);
}
Server::Server(Server&& other) : socket(std::move(other.socket)) {}
Server& Server::operator=(Server&& other) {
    self.socket = std::move(other.socket);
    return self;
}

void Server::listen() {
    self.socket.init();
    self.socket.listen();
}

void Server::on_receive(std::function<std::vector<Response>(Request&&)> func) {
    self.socket.on_receive([this, func](std::string_view raw_input)
                               -> std::optional<std::vector<Response>> {
        std::optional<Request> request = Request::create(raw_input);
        if (!request.has_value()) {
            LOG_ERROR("Invalid request: {}", raw_input);
            return std::nullopt;
        }

        std::vector<Response> responses = func(std::move(request.value()));

        return std::optional(std::move(responses));
    });
}

std::string_view Server::get_host() const noexcept {
    return self.config.host;
}

}  // namespace http
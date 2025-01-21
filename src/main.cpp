#include "uni_build.hpp"

#include "server.hpp"
#include "string_utils.hpp"

int main() {
    try {
        http::ServerConfig config{
            .host = "localhost",
            .port = 3000,
        };
        http::Server server = http::Server(config);
        server.on_receive(
            [&server](http::Request&& request) -> std::vector<http::Response> {
                LOG_INFO("{} {}", request.method_to_string(), request.route);
                LOG_INFO("body: {}", request.body);
                LOG_INFO("");

                std::vector<http::Response> responses{};

                http::Response response =
                    http::Response::create(server, request, http::HttpCode::Ok,
                        http::ContentType::Text, "Hello World!");

                responses.push_back(response);

                return responses;
            });

        LOG_INFO(
            "Server is listening at http://{}:{}", config.host, config.port);

        server.listen();
    } catch (std::exception& e) {
        LOG_ERROR("{}", e.what());
        return 1;
    }

    return 0;
}

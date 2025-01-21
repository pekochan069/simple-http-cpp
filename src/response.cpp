#include "response.hpp"
#include "log.hpp"
#include "request.hpp"
#include "server.hpp"
#include "string_utils.hpp"

namespace http {

Response Response::create(const Server& server, const Request& request,
    HttpCode http_code, ContentType content_type, std::string_view body) {
    LOG_TRACE("http::Response::create()");
    Response response{};

    response.http_version = request.http_version;
    response.http_code = http_code;
    response.body = body;

    // Fields
    response.fields.insert(
        {"Location", std::format("{}{}", server.get_host(), request.route)});
    response.fields.insert(
        {"Content-Type", std::string(get_content_type(content_type))});
    ContentTypeCategory category = get_content_type_category(content_type);
    switch (category) {
        case ContentTypeCategory::Text: {
            response.fields.insert(
                {"Content-Length", std::to_string(body.size())});
            break;
        }
        case ContentTypeCategory::Application: {
            if (content_type == ContentType::JSON) {
                response.fields.insert(
                    {"Content-Length", std::to_string(body.size())});
            }
            break;
        }
        default:
    }

    return response;
}

std::string Response::response_to_message(const Response& response) noexcept {
    LOG_TRACE("http::Response::get_full_message()");
    std::vector<std::string> message;

    // Status Line
    message.push_back(
        std::format("{} {}", http_version_to_string(response.http_version),
            http_code_to_string(response.http_code)));
    // Fields
    for (const auto& [key, value] : response.fields) {
        message.push_back(std::format("{}: {}", key, value));
    }

    // Body
    message.push_back("");
    message.push_back(std::string(response.body));

    return join(message, "\r\n");
}

}  // namespace http
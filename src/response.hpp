#pragma once
#include <string>
#include <string_view>
#include <unordered_map>
#include "content_type.hpp"
#include "http_code.hpp"
#include "http_method.hpp"
#include "http_version.hpp"

namespace http {

class Server;
class Request;

struct Response {
    HttpVersion http_version;
    HttpCode http_code;
    std::unordered_map<std::string, std::string> fields;
    std::string_view body;

    static Response create(const Server& server, const Request& request,
        HttpCode http_code, ContentType content_type, std::string_view body);

    static std::string response_to_message(const Response& response) noexcept;
};

}  // namespace http
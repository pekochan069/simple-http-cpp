#pragma once
#include "log.hpp"

namespace http {

enum struct HttpVersion { Http1_1, Unknown };

HttpVersion parse_http_version(std::string_view raw_version) {
    LOG_TRACE("http::parse_http_version()");
    std::string_view version_literal(
        raw_version.begin() + 4, raw_version.end());

    using namespace std::string_view_literals;
    if (version_literal == "1.1"sv) {
        return HttpVersion::Http1_1;
    } else {
        return HttpVersion::Unknown;
    }
}

std::string_view http_version_to_string(HttpVersion http_version) {
    LOG_TRACE("http::http_version_to_string()");
    using namespace std::string_view_literals;
    switch (http_version) {
        case HttpVersion::Http1_1:
            return "HTTP/1.1"sv;
        default:
            return "HTTP/1.1"sv;
    }
}

}
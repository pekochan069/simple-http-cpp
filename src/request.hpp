#pragma once
#include <algorithm>
#include <optional>
#include <ranges>
#include <string_view>
#include <unordered_map>
#include "log.hpp"
#include "string_utils.hpp"

namespace http {

enum struct Method {
    Get,
    Post,
    Patch,
    Put,
    Delete,
    Head,
    Connect,
    Options,
    Trace,
    Unknown
};

enum struct HttpVersion { Http1_1, Unknown };

Method parse_method(std::string_view method) {
    LOG_TRACE("http::parse_method()");
    using namespace std::string_view_literals;
    if (method == "GET"sv) {
        return Method::Get;
    } else if (method == "POST"sv) {
        return Method::Post;
    } else if (method == "PATCH"sv) {
        return Method::Patch;
    } else if (method == "PUT"sv) {
        return Method::Put;
    } else if (method == "DELETE"sv) {
        return Method::Delete;
    } else if (method == "HEAD"sv) {
        return Method::Head;
    } else if (method == "CONNECT"sv) {
        return Method::Connect;
    } else if (method == "OPTIONS"sv) {
        return Method::Options;
    } else if (method == "TRACE"sv) {
        return Method::Trace;
    } else {
        return Method::Unknown;
    }
}

std::string_view method_to_string(Method method) {
    LOG_TRACE("http::method_to_string()");
    using namespace std::string_view_literals;
    switch (method) {
        case Method::Get:
            return "GET"sv;
        case Method::Post:
            return "POST"sv;
        case Method::Put:
            return "PUT"sv;
        case Method::Patch:
            return "PATCH"sv;
        case Method::Delete:
            return "DELETE"sv;
        case Method::Connect:
            return "CONNECT"sv;
        case Method::Head:
            return "HEAD"sv;
        case Method::Options:
            return "OPTIONS"sv;
        case Method::Trace:
            return "TRACE"sv;
        default:
            return "GET";
    }
}

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
            return "HTTP/1.1";
        case HttpVersion::Unknown:
            return "HTTP/1.1";
    }
}

struct Request {
    Method method;
    std::string_view request_target;
    std::string_view route;
    HttpVersion http_version;
    std::unordered_map<std::string_view, std::string_view> fields;
    std::string_view body;

    static std::optional<Request> create(std::string_view raw_input) {
        LOG_TRACE("http::Request::create()");
        Request request{};

        if (raw_input == "") {
            return std::nullopt;
        }

        auto split_input =
            split(raw_input, "\r\n") | std::views::split(std::string_view{""});

        auto header = split_input.front();

        // Request line
        // GET /route HTTP/1.1
        std::string_view request_line = header.front();
        auto split_request_line = split(request_line, ' ');
        auto split_request_line_it = split_request_line.begin();
        request.method = parse_method(*(split_request_line_it++));
        if (split_request_line_it == split_request_line.end()) {
            return std::nullopt;
        }
        request.route = *(split_request_line_it++);
        if (split_request_line_it == split_request_line.end()) {
            return std::nullopt;
        }
        request.http_version = parse_http_version(*split_request_line_it);

        // Fields
        // field-name : field-value
        auto fields = header | std::views::drop(1) |
                      std::views::transform([](std::string_view field) {
                          auto split_field = split(field, ':');
                          auto field_name = split_field.front();
                          auto temp_field_value =
                              (split_field | std::views::drop(1)).front();
                          auto field_value = temp_field_value.substr(
                              temp_field_value.find_first_not_of(' '),
                              temp_field_value.find_last_not_of(' '));

                          return std::pair{field_name, field_value};
                      });
        request.fields.insert_range(fields);

        // Body
        request.body = (split_input | std::views::drop(1)).front().front();

        return std::optional(request);
    }

    std::string_view method_to_string() {
        LOG_TRACE("http::Request::method_to_string()");
        return http::method_to_string(self.method);
    }

    std::string_view http_version_to_string() {
        LOG_TRACE("http::Request::http_version_to_string()");
        return http::http_version_to_string(self.http_version);
    }

private:
    Request& self = *this;
};

}  // namespace http
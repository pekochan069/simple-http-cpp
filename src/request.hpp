#pragma once
#include <algorithm>
#include <optional>
#include <ranges>
#include <string_view>
#include <unordered_map>
#include "http_method.hpp"
#include "http_version.hpp"
#include "log.hpp"
#include "string_utils.hpp"

namespace http {

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
        auto sv = std::ranges::to<std::vector>(split_request_line);

        if (sv.size() != 3) {
            return std::nullopt;
        }
        request.method = parse_method(sv[0]);
        request.route = sv[1];
        request.http_version = parse_http_version(sv[2]);

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
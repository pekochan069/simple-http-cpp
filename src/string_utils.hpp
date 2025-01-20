#pragma once

#include <ranges>
#include <string>
#include <string_view>

namespace http {

auto split(std::string_view str, char delimiter) {
    LOG_TRACE("http::split()");
    return str | std::views::split(delimiter) |
           std::views::transform(
               [](const auto& i) { return std::string_view(i); });
}

auto split(std::string_view str, std::string_view delimiter) {
    LOG_TRACE("http::split()");
    return str | std::views::split(delimiter) |
           std::views::transform(
               [](const auto& i) { return std::string_view(i); });
}

std::string to_lowercase(std::string_view str) {
    LOG_TRACE("http::to_lowercase()");
    std::string new_str(str);

    for (char& c : new_str) {
        c = std::toupper(c);
    }

    return new_str;
}

std::string to_uppercase(std::string_view str) {
    LOG_TRACE("http::to_uppercase()");
    std::string new_str(str);

    for (char& c : new_str) {
        c = std::toupper(c);
    }

    return new_str;
}

}  // namespace http
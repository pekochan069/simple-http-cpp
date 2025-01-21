#pragma once

#include <ranges>
#include <string>
#include <string_view>

namespace http {

auto split(std::string_view str, char delimiter) noexcept {
    LOG_TRACE("http::split()");
    return str | std::views::split(delimiter) |
           std::views::transform(
               [](const auto& i) { return std::string_view(i); });
}

auto split(std::string_view str, std::string_view delimiter) noexcept {
    LOG_TRACE("http::split()");
    return str | std::views::split(delimiter) |
           std::views::transform(
               [](const auto& i) { return std::string_view(i); });
}

std::string join(
    const std::vector<std::string>& parts, const char* delimiter) noexcept {
    if (parts.size() == 0) {
        return "";
    } else if (parts.size() == 1) {
        return parts[0];
    }

    std::string out = "";

    for (size_t i = 0; i < parts.size(); ++i) {
        out += parts[i];

        if (i != parts.size() - 1) {
            out += delimiter;
        }
    }

    return out;
}

std::string to_lowercase(std::string_view str) noexcept {
    LOG_TRACE("http::to_lowercase()");
    std::string new_str(str);

    for (char& c : new_str) {
        c = std::toupper(c);
    }

    return new_str;
}

std::string to_uppercase(std::string_view str) noexcept {
    LOG_TRACE("http::to_uppercase()");
    std::string new_str(str);

    for (char& c : new_str) {
        c = std::toupper(c);
    }

    return new_str;
}

}  // namespace http
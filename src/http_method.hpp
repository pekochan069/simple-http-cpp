#pragma once
#include "log.hpp"

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

}
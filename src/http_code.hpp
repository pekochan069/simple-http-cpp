#pragma once
#include <iostream>
#include <string_view>
#include "log.hpp"

namespace http {

enum struct HttpCode {
    // Information
    Continue = 100,
    SwitchingProtocol = 101,
    EarlyHints = 103,
    // Successful
    Ok = 200,
    Created = 201,
    Accepted = 202,
    NonAuthoritativeInformation = 203,
    NoContent = 204,
    ResetContent = 205,
    PartialContent = 206,
    MultiStatus = 207,
    AlreadyReported = 208,
    IMUsed = 226,
    // Redirection
    MultipleChoices = 300,
    MovedPermanently = 301,
    Found = 302,
    SeeOther = 303,
    NotModified = 304,
    Unused = 306,
    TemporaryRedirect = 307,
    PermanentRedirect = 308,
    // Client Error
    BadRequest = 400,
    Unauthorized = 401,
    PaymentRequired = 402,
    Forbidden = 403,
    NotFound = 404,
    MethodNotAllowed = 405,
    NotAcceptable = 406,
    ProxyAuthenticationRequired = 407,
    RequestTimeout = 408,
    Conflict = 409,
    Gone = 410,
    LengthRequired = 411,
    PreconditionFailed = 412,
    ContentTooLarge = 413,
    URITooLong = 414,
    UnsupportedMediaType = 415,
    RangeNotSatisfiable = 416,
    ExpectationFailed = 417,
    ImaTeapot = 418,
    MisdirectedRequest = 421,
    UnprocessableContent = 422,
    Locked = 423,
    FailedDependency = 424,
    TooEarly = 425,
    UpgradeRequired = 426,
    PreconditionRequired = 428,
    TooManyRequests = 429,
    RequestHeaderFieldsTooLarge = 431,
    UnavailableForLegalReasons = 451,
    // Server Error
    InternalServerError = 500,
    NotImplemented = 501,
    BadGateway = 502,
    ServiceUnavailable = 503,
    GatewayTimeout = 504,
    HTTPVersionNotSupported = 505,
    VariantAlsoNegotiates = 506,
    InsufficientStorage = 507,
    LoopDetected = 508,
    NotExtended = 510,
    NetworkAuthenticationRequired = 511
};

std::string http_code_to_string(HttpCode http_code) {
    LOG_TRACE("http::http_code_to_string()");

    switch (http_code) {
        case HttpCode::Continue:
            return "100 Continue";
        case HttpCode::SwitchingProtocol:
            return "101 Switching Protocol";
        case HttpCode::EarlyHints:
            return "103 Early Hints";
        case HttpCode::Ok:
            return "200 OK";
        case HttpCode::Created:
            return "201 Created";
        case HttpCode::Accepted:
            return "202 Accepted";
        case HttpCode::NonAuthoritativeInformation:
            return "203 Non Authoritative Information";
        case HttpCode::NoContent:
            return "204 No Content";
        case HttpCode::ResetContent:
            return "205 Reset Content";
        case HttpCode::PartialContent:
            return "206 Partial Content";
        case HttpCode::MultiStatus:
            return "207 Multi Status";
        case HttpCode::AlreadyReported:
            return "208 Already Reported";
        case HttpCode::IMUsed:
            return "226 IM Used";
        case HttpCode::MultipleChoices:
            return "300 Multiple Choices";
        case HttpCode::MovedPermanently:
            return "301 Moved Permanently";
        case HttpCode::Found:
            return "302 Found";
        case HttpCode::SeeOther:
            return "303 See Other";
        case HttpCode::NotModified:
            return "304 Not Modified";
        case HttpCode::Unused:
            return "306 Unused";
        case HttpCode::TemporaryRedirect:
            return "307 Temporary Redirect";
        case HttpCode::PermanentRedirect:
            return "308 Permanent Redirect";
        case HttpCode::BadRequest:
            return "400 Bad Request";
        case HttpCode::Unauthorized:
            return "401 Unauthorized";
        case HttpCode::PaymentRequired:
            return "402 Payment Required";
        case HttpCode::Forbidden:
            return "403 Forbidden";
        case HttpCode::NotFound:
            return "404 Not Found";
        case HttpCode::MethodNotAllowed:
            return "405 Method Not Allowed";
        case HttpCode::NotAcceptable:
            return "406 Not Acceptable";
        case HttpCode::ProxyAuthenticationRequired:
            return "407 Proxy Authentication Required";
        case HttpCode::RequestTimeout:
            return "408 Request Timeout";
        case HttpCode::Conflict:
            return "409 Conflict";
        case HttpCode::Gone:
            return "410 Gone";
        case HttpCode::LengthRequired:
            return "411 Length Required";
        case HttpCode::PreconditionFailed:
            return "412 Precondition Failed";
        case HttpCode::ContentTooLarge:
            return "413 Content Too Large";
        case HttpCode::URITooLong:
            return "414 URI Too Long";
        case HttpCode::UnsupportedMediaType:
            return "415 Unsupported Media Type";
        case HttpCode::RangeNotSatisfiable:
            return "416 Range Not Satisfiable";
        case HttpCode::ExpectationFailed:
            return "417 Expectation Failed";
        case HttpCode::ImaTeapot:
            return "418 I'm a teapot";
        case HttpCode::MisdirectedRequest:
            return "421 Misdirected Request";
        case HttpCode::UnprocessableContent:
            return "422 Unprocessable Content";
        case HttpCode::Locked:
            return "423 Locked";
        case HttpCode::FailedDependency:
            return "424 Failed Dependency";
        case HttpCode::TooEarly:
            return "425 Too Early";
        case HttpCode::UpgradeRequired:
            return "426 Upgrade Required";
        case HttpCode::PreconditionRequired:
            return "428 Precondition Required";
        case HttpCode::TooManyRequests:
            return "429 Too Many Requests";
        case HttpCode::RequestHeaderFieldsTooLarge:
            return "431 Request Header Fields Too Large";
        case HttpCode::UnavailableForLegalReasons:
            return "451 Unavailable For Legal Reasons";
        case HttpCode::InternalServerError:
            return "500 Internal Server Error";
        case HttpCode::NotImplemented:
            return "501 Not Implemented";
        case HttpCode::BadGateway:
            return "502 Bad Gateway";
        case HttpCode::ServiceUnavailable:
            return "503 Service Unavailable";
        case HttpCode::GatewayTimeout:
            return "504 Gateway Timeout";
        case HttpCode::HTTPVersionNotSupported:
            return "505 HTTP Version Not Supported";
        case HttpCode::VariantAlsoNegotiates:
            return "506 Variant Also Negotiates";
        case HttpCode::InsufficientStorage:
            return "507 Insufficient Storage";
        case HttpCode::LoopDetected:
            return "508 Loop Detected";
        case HttpCode::NotExtended:
            return "510 Not Extended";
        case HttpCode::NetworkAuthenticationRequired:
            return "511 Network Authentication Required";
        default: {
            std::unreachable();
        }
    }
}

HttpCode code_to_HttpCode(uint8_t code) {
    LOG_TRACE("http::code_to_HttpCode()");
    switch (code) {
        case 100:
            return HttpCode::Continue;
        case 101:
            return HttpCode::SwitchingProtocol;
        case 103:
            return HttpCode::EarlyHints;
        case 200:
            return HttpCode::Ok;
        case 201:
            return HttpCode::Created;
        case 202:
            return HttpCode::Accepted;
        case 203:
            return HttpCode::NonAuthoritativeInformation;
        case 204:
            return HttpCode::NoContent;
        case 205:
            return HttpCode::ResetContent;
        case 206:
            return HttpCode::PartialContent;
        case 207:
            return HttpCode::MultiStatus;
        case 208:
            return HttpCode::AlreadyReported;
        case 226:
            return HttpCode::IMUsed;
        case 300:
            return HttpCode::MultipleChoices;
        case 301:
            return HttpCode::MovedPermanently;
        case 302:
            return HttpCode::Found;
        case 303:
            return HttpCode::SeeOther;
        case 304:
            return HttpCode::NotModified;
        case 306:
            return HttpCode::Unused;
        case 307:
            return HttpCode::TemporaryRedirect;
        case 308:
            return HttpCode::PermanentRedirect;
        case 400:
            return HttpCode::BadRequest;
        case 401:
            return HttpCode::Unauthorized;
        case 402:
            return HttpCode::PaymentRequired;
        case 403:
            return HttpCode::Forbidden;
        case 404:
            return HttpCode::NotFound;
        case 405:
            return HttpCode::MethodNotAllowed;
        case 406:
            return HttpCode::NotAcceptable;
        case 407:
            return HttpCode::ProxyAuthenticationRequired;
        case 408:
            return HttpCode::RequestTimeout;
        case 409:
            return HttpCode::Conflict;
        case 410:
            return HttpCode::Gone;
        case 411:
            return HttpCode::LengthRequired;
        case 412:
            return HttpCode::PreconditionFailed;
        case 413:
            return HttpCode::ContentTooLarge;
        case 414:
            return HttpCode::URITooLong;
        case 415:
            return HttpCode::UnsupportedMediaType;
        case 416:
            return HttpCode::RangeNotSatisfiable;
        case 417:
            return HttpCode::ExpectationFailed;
        case 418:
            return HttpCode::ImaTeapot;
        case 421:
            return HttpCode::MisdirectedRequest;
        case 422:
            return HttpCode::UnprocessableContent;
        case 423:
            return HttpCode::Locked;
        case 424:
            return HttpCode::FailedDependency;
        case 425:
            return HttpCode::TooEarly;
        case 426:
            return HttpCode::UpgradeRequired;
        case 428:
            return HttpCode::PreconditionRequired;
        case 429:
            return HttpCode::TooManyRequests;
        case 431:
            return HttpCode::RequestHeaderFieldsTooLarge;
        case 451:
            return HttpCode::UnavailableForLegalReasons;
        case 500:
            return HttpCode::InternalServerError;
        case 501:
            return HttpCode::NotImplemented;
        case 502:
            return HttpCode::BadGateway;
        case 503:
            return HttpCode::ServiceUnavailable;
        case 504:
            return HttpCode::GatewayTimeout;
        case 505:
            return HttpCode::HTTPVersionNotSupported;
        case 506:
            return HttpCode::VariantAlsoNegotiates;
        case 507:
            return HttpCode::InsufficientStorage;
        case 508:
            return HttpCode::LoopDetected;
        case 510:
            return HttpCode::NotExtended;
        case 511:
            return HttpCode::NetworkAuthenticationRequired;
        default:
            std::unreachable();
    }
}

}  // namespace http
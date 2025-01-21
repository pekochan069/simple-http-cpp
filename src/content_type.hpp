#pragma once
#include <string_view>
#include "log.hpp"

namespace http {

enum struct ContentType {
    Text,
    Html,
    Javascript,
    Css,
    Csv,
    JSON,
    Xml,
    Bin,
    Pdf,
    Jpeg,
    Png,
    Svg,
    Webp,
    Ico,
    Mp3,
    Wav,
    Weba,
    Mp4,
    Mpeg,
    Webm,
};

enum struct ContentTypeCategory { Text, Application, Image, Audio, Video };

std::string_view get_content_type(ContentType content_type) {
    LOG_TRACE("http::get_content_type()");
    using namespace std::string_view_literals;
    switch (content_type) {
        case ContentType::Text:
            return "text/plain"sv;
        case ContentType::Html:
            return "text/html"sv;
        case ContentType::Javascript:
            return "text/javascript"sv;
        case ContentType::Css:
            return "text/css"sv;
        case ContentType::Csv:
            return "text/csv"sv;
        case ContentType::JSON:
            return "application/json"sv;
        case ContentType::Xml:
            return "application/xml"sv;
        case ContentType::Bin:
            return "application/octet-stream"sv;
        case ContentType::Pdf:
            return "application/pdf"sv;
        case ContentType::Jpeg:
            return "image/jpeg"sv;
        case ContentType::Png:
            return "image/png"sv;
        case ContentType::Svg:
            return "image/svg+xml"sv;
        case ContentType::Webp:
            return "image/webp"sv;
        case ContentType::Ico:
            return "image/vnd.microsoft.icon"sv;
        case ContentType::Mp3:
            return "audio/mpeg"sv;
        case ContentType::Wav:
            return "audio/wav"sv;
        case ContentType::Weba:
            return "audio/webm"sv;
        case ContentType::Mp4:
            return "video/mp4"sv;
        case ContentType::Mpeg:
            return "video/mpeg"sv;
        case ContentType::Webm:
            return "video/webm"sv;
        default:
            std::unreachable();
    }
}

ContentTypeCategory get_content_type_category(ContentType content_type) {
    LOG_TRACE("http::get_content_type_category()");
    switch (content_type) {
        case ContentType::Text:
        case ContentType::Html:
        case ContentType::Javascript:
        case ContentType::Css:
        case ContentType::Csv:
            return ContentTypeCategory::Text;
        case ContentType::JSON:
        case ContentType::Xml:
        case ContentType::Bin:
        case ContentType::Pdf:
            return ContentTypeCategory::Application;
        case ContentType::Jpeg:
        case ContentType::Png:
        case ContentType::Svg:
        case ContentType::Webp:
        case ContentType::Ico:
            return ContentTypeCategory::Image;
        case ContentType::Mp3:
        case ContentType::Wav:
        case ContentType::Weba:
            return ContentTypeCategory::Audio;
        case ContentType::Mp4:
        case ContentType::Mpeg:
        case ContentType::Webm:
            return ContentTypeCategory::Video;
        default:
            std::unreachable();
    }
}

}  // namespace http
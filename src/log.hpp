#include <print>

namespace http {

#ifdef LOG_LEVEL_TRACE
    #define LOG_TRACE(...) std::println(__VA_ARGS__)
    #define LOG_INFO(...) std::println(__VA_ARGS__)
    #define LOG_WARN(...) std::println(__VA_ARGS__)
    #define LOG_ERROR(...) std::println(__VA_ARGS__)
#elif LOG_LEVEL_INFO
    #define LOG_TRACE(...)
    #define LOG_INFO(...) std::println(__VA_ARGS__)
    #define LOG_WARN(...) std::println(__VA_ARGS__)
    #define LOG_ERROR(...) std::println(__VA_ARGS__)
#elif LOG_LEVEL_WARN
    #define LOG_TRACE(...)
    #define LOG_INFO(...)
    #define LOG_WARN(...) std::println(__VA_ARGS__)
    #define LOG_ERROR(...) std::println(__VA_ARGS__)
#elif LOG_LEVEL_ERROR
    #define LOG_TRACE(...)
    #define LOG_INFO(...)
    #define LOG_WARN(...)
    #define LOG_ERROR(...) std::println(__VA_ARGS__)
#else
    #define LOG_TRACE(...)
    #define LOG_INFO(...)
    #define LOG_WARN(...)
    #define LOG_ERROR(...)
#endif

}  // namespace http
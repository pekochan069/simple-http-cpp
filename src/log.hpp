#include <print>

namespace http {
#ifdef _DEBUG
    #define LOG(...) std::println(__VA_ARGS__)
#else
    #define LOG(...)
#endif

#define LOG_TRACE(...) std::println(__VA_ARGS__)
#define LOG_INFO(...) std::println(__VA_ARGS__)
#define LOG_WARN(...) std::println(__VA_ARGS__)
#define LOG_ERROR(...) std::println(__VA_ARGS__)

}
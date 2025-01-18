#include <print>

#ifdef _DEBUG
    #define LOG(...) std::println(__VA_ARGS__)
#else
    #define LOG(...)
#endif

#define TRACE(...) std::println(__VA_ARGS__)
#define INFO(...) std::println(__VA_ARGS__)
#define WARN(...) std::println(__VA_ARGS__)
#define ERROR(...) std::println(__VA_ARGS__)
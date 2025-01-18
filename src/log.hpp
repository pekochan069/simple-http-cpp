#include <print>

#ifdef _DEBUG
    #define LOG(...) std::println(__VA_ARGS__)
#else
    #define LOG(...) 
#endif

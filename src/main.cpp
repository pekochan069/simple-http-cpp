#ifdef _WIN32
    #ifndef WIN32_LEAN_AND_MEAN
        #define WIN32_LEAN_AND_MEAN
    #endif

    #include <windows.h>

    #include <WinSock2.h>
#endif

#include "server.hpp"
#include "string_utils.hpp"

int main() {
    try {
        http::Server server = http::Server{3000};
        server.on_receive([](http::Request&& request) {
            LOG_INFO("{} {}", request.method_to_string(), request.route);
            std::println();
        });
        server.listen();
    } catch (std::exception& e) {
        LOG_ERROR("{}", e.what());
        return 1;
    }

    return 0;
}

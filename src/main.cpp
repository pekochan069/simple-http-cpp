#ifdef _WIN32
    #ifndef WIN32_LEAN_AND_MEAN
        #define WIN32_LEAN_AND_MEAN
    #endif

    #include <windows.h>

    #include <WinSock2.h>
#endif

#include "socket.hpp"

int main() {
    http::SocketConfig config{};
    http::Socket socket = http::Socket();

    try {
        socket.init();
        socket.on_receive([](SOCKET socket, std::string_view view) {
            std::println("{}", view);
        });
        socket.on_connect(
            [](SOCKET socket) { std::println("Connect: {}", socket); });
        socket.on_disconnect(
            [](SOCKET socket) { std::println("Disconnect: {}", socket); });
        socket.accept();
        socket.terminate();
    } catch (std::exception& e) {
        std::println("{}", e.what());
        return 1;
    }

    return 0;
}

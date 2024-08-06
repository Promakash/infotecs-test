#include "socketserver.h"

int main() {
    constexpr int buffer_size = 192;
    constexpr int port = 8000;
    SocketServer serv(buffer_size, port);
    serv.start();
}

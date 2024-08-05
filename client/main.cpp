#include "processor.h"

int main() {
    std::string ip = "127.0.0.1";
    constexpr int port = 8000;
    StringProcessor serv(ip, port);

    serv.Run();
    return 0;
}

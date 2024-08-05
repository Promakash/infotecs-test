#include "socketserver.h"

int main() {
    SocketServer serv(192, 8000);
    serv.start();
}

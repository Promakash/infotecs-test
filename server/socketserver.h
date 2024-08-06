#pragma once
#include "handler.h"
#include <iostream>
#include <vector>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include "unistd.h"

class SocketServer {
public:
    SocketServer(const int buffer_size, const int port);

    ~SocketServer();

    void start();

private:
    const int address_family_ = AF_INET;
    const int clients_count_ = 1;
    int buf_size_;
    int sock_, client_, port_;
    std::vector<char> buffer_;
    sockaddr_in server_addr_;
    Handler handler_;

    bool socket_init();

    bool bind_socket();
};

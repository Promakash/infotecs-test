#pragma once

#include <iostream>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "unistd.h"

class SocketClient {
public:
    SocketClient(std::string &ip, const int port);

    ~SocketClient();

    void setup_client();

    void send_msg(std::string &msg);

private:
    const int address_family_ = AF_INET;
    int sockfd_, port_;
    std::string ip_;
    sockaddr_in server_addr_;
    bool active_connection_;

    void socket_init();

    void connect_to_sock();

    bool check_error();

    bool ensure_connection();
};

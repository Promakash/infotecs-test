#pragma once

#include <iostream>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include "unistd.h"

class SocketClient {
private:
    const int address_family_ = AF_INET;
    bool active_connection_;
    bool has_connected_;
    int sockfd_, port_;
    std::string ip_;
    sockaddr_in server_addr_;

public:
    SocketClient(std::string &ip, const int port)
        : ip_(std::move(ip)), port_(port), active_connection_(false), has_connected_(false), sockfd_(-1) {}

    ~SocketClient() {
        close(sockfd_);
    }

private:
    void socket_init() {
        sockfd_ = socket(address_family_, SOCK_STREAM, 0);
        if (sockfd_ < 0) {
            std::cerr << "Can't create socket" << '\n';
            exit(1);
        }
        server_addr_.sin_family = address_family_;
        server_addr_.sin_port = htons(port_);
        if (inet_pton(AF_INET, ip_.data(), &server_addr_.sin_addr) <= 0) {
            std::cerr << "Can't convert ip to bytes" << '\n';
            exit(1);
        }
    }

    void connect_to_sock() {
        active_connection_ = connect(sockfd_, reinterpret_cast<sockaddr *>(&server_addr_), sizeof(server_addr_)) >= 0;
        if (!active_connection_) {
            return;
        }
        has_connected_ = true;
    }

    bool check_error() {
        int error = 0;
        socklen_t len = sizeof(error);
        int retval = getsockopt(sockfd_, SOL_SOCKET, SO_ERROR, &error, &len);

        if (retval != 0) {
            std::cerr << "Error getting socket error code: " << strerror(retval) << '\n';
            return true;
        }

        if (error != 0) {
            std::cerr << "Socket error: " << strerror(error) << '\n';
            return true;
        }
        return false;
    }

    bool ensure_connection() {
        if (!active_connection_) {
            if (sockfd_ <= 0) {
                setup_client();
            } else {
                connect_to_sock();
            }
        }
        return active_connection_ && !check_error();
    }

public:
    void setup_client() {
        socket_init();
        connect_to_sock();
    }

    void send_msg(std::string &msg) {
        if (!ensure_connection()) {
            std::cerr << "No connection with server - can't send message" << '\n';
            if (has_connected_) {
                sockfd_ = close(sockfd_);
                has_connected_ = false;
            }
            active_connection_ = false;
            return;
        }

        ssize_t res = write(sockfd_, msg.data(), msg.size());
        if (res != msg.size()) {
            std::cerr << "Error while sending message!" << '\n';
            active_connection_ = false;
        } else {
            std::cout << "Message has been sent successfully!" << '\n';
        }
    }
};

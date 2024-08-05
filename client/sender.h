#pragma once

#include <iostream>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include "unistd.h"

class Client {
private:
    const int address_family_ = AF_INET;
    bool active_connection;
    bool has_connected;
public:
    Client(std::string &ip, const int port) {
        ip_ = std::move(ip);
        port_ = port;
        active_connection = false;
        has_connected = false;
    }

    ~Client() {
        close(client_);
    }

private:
    void socket_init() {
        client_ = socket(address_family_, SOCK_STREAM, 0);
        if (client_ < 0) {
            std::cerr << "Can't create socket" << '\n';
            exit(1);
        }
        std::cout << "Socket has been created" << '\n';
        server_addr.sin_family = address_family_;
        server_addr.sin_port = htons(port_);
        if (inet_pton(AF_INET, ip_.data(), &server_addr.sin_addr) <= 0) {
            std::cerr << "Can't translate to bytes ip" << '\n';
            exit(1);
        }
    }

    void connect_to_sock() {
        active_connection = connect(client_, reinterpret_cast<sockaddr *>(&server_addr), sizeof(server_addr)) >= 0;
        if (!active_connection) {
            std::cerr << "Can't connect to server" << '\n';
            return;
        }
        std::cout << "Connected to the server!" << '\n';
        has_connected = true;
    }

    bool CheckErr() {
        int error = 0;
        socklen_t len = sizeof (error);
        int retval = getsockopt (client_, SOL_SOCKET, SO_ERROR, &error, &len);

        if (retval != 0) {
            /* there was a problem getting the error code */
            fprintf(stderr, "error getting socket error code: %s\n", strerror(retval));
            return true;
        }

        if (error != 0) {
            /* socket has a non zero error status */
            fprintf(stderr, "socket error: %s\n", strerror(error));
            return true;
        }
        return false;
    }

public:
    void setup_client() {
        socket_init();
        connect_to_sock();
    }

    void send_msg(std::string &msg) {
        if (!active_connection) {
            if (client_ <= 0) {
                setup_client();
            }
            else {
                connect_to_sock();
            }
        }
        if (!active_connection || CheckErr()) {
            std::cerr << "No connection with server!" << '\n';
            if (has_connected) {
                client_ = close(client_);
                has_connected = false;
            }
            active_connection = false;
            return;
        }
        ssize_t res = write(client_, msg.data(), msg.size());
        if (res != msg.size()) {
            std::cerr << "Error while sending message!" << '\n';
            active_connection = false;
        } else {
            std::cout << "Message has been sent successfully!" << '\n';
        }
    }

    int client_, port_;
    socklen_t sock_size_;
    std::string ip_;
    sockaddr_in server_addr;
};

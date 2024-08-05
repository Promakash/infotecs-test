#include "handler.h"
#include <iostream>
#include <vector>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include "unistd.h"

class Server {
private:
    const int address_family_ = AF_INET;
    const int clients_count_ = 1;
    int sock_, client_, port_;
    size_t buf_size_;
    std::vector<char> buffer_;
    sockaddr_in server_addr;
    handler handler_;
public:
    Server(const int buffer_size, const int port)
        : buf_size_(buffer_size), port_(port) {
        buffer_.resize(buffer_size);
    }

    ~Server() {
        close(sock_);
    }

    void socket_init() {
        sock_ = socket(address_family_, SOCK_STREAM, 0);
        if (sock_ < 0) {
            std::cerr << "Can't create socket" << '\n';
            exit(1);
        }
    }

    bool bind_socket() {
        server_addr.sin_family = address_family_;
        server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
        server_addr.sin_port = htons(port_);
        return bind(sock_, reinterpret_cast<sockaddr*>(&server_addr), sizeof(server_addr)) < 0;
    }

    void start() {
        if (listen(sock_, clients_count_) < 0) {
            std::cerr << "Listen err" << '\n';
            exit(1);
        }

        while (true) {
            int n = 1;
            client_ = accept(sock_, nullptr, nullptr);
            std::cout << "new client or what i dunno: " << client_ << '\n';
            while ((n = read(client_, buffer_.data(), buf_size_) > 0)) {
                handler_.handle_reponse(buffer_);
                memset(buffer_.data(), 0, buffer_.size());
            }
        }
    }
};

int main() {
    Server serv(192, 8000);
    serv.socket_init();
    if (serv.bind_socket()) {
        std::cout << "can't bind socket!" << '\n';
        return -1;
    }
    while (true) {
        std::cout << "Server started" << '\n';
        serv.start();
    }
}
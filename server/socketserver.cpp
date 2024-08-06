#include "socketserver.h"

SocketServer::SocketServer(const int buffer_size, const int port)
    : buf_size_(buffer_size), port_(port) {
    buffer_.resize(buffer_size);
}

SocketServer::~SocketServer() {
    close(sock_);
    close(client_);
}

void SocketServer::start() {
    if (!socket_init()) {
        return;
    }
    if (!bind_socket()) {
        return;
    }
    if (listen(sock_, clients_count_) < 0) {
        std::cerr << "Listen err" << '\n';
        return;
    }

    while (true) {
        int n = 1;
        client_ = accept(sock_, nullptr, nullptr);
        std::cout << "client has connected" << '\n';
        while ((n = read(client_, buffer_.data(), buf_size_)) > 0) {
            handler_.handle_reponse(buffer_, n);
            std::fill(buffer_.begin(), buffer_.end(), 0);
        }
        std::cout << "client has disconnected!" << '\n';
        close(client_);
    }
}

bool SocketServer::socket_init() {
    sock_ = socket(address_family_, SOCK_STREAM, 0);
    if (sock_ < 0) {
        std::cerr << "Can't create socket" << '\n';
        return false;
    }
    const int enable = 1;
    if (setsockopt(sock_, SOL_SOCKET, SO_REUSEPORT, &enable, sizeof(int)) < 0) {
        std::cerr << "setting SO_REUSEADDR failed" << '\n';
        return false;
    }
    if (setsockopt(sock_, SOL_SOCKET, SO_REUSEPORT, &enable, sizeof(int)) < 0) {
        std::cerr << "setting SO_REUSEPORT failed" << '\n';
        return false;
    }

    return true;
}

bool SocketServer::bind_socket() {
    server_addr_.sin_family = address_family_;
    server_addr_.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr_.sin_port = htons(port_);
    if (bind(sock_, reinterpret_cast<sockaddr *>(&server_addr_), sizeof(server_addr_)) < 0) {
        std::cout << "can't bind socket!" << '\n';
        return false;
    }
    return true;
}

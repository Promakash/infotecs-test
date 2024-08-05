#include "socketclient.h"

SocketClient::SocketClient(std::string &ip, const int port)
    : sockfd_(-1), port_(port), ip_(std::move(ip)), active_connection_(false) {
}

SocketClient::~SocketClient() {
    close(sockfd_);
}

void SocketClient::setup_client() {
    socket_init();
    connect_to_sock();
}

void SocketClient::send_msg(std::string &msg) {
    if (!ensure_connection()) {
        std::cerr << "No connection with server - can't send message" << '\n';
        active_connection_ = false;
        return;
    }

    size_t res = write(sockfd_, msg.data(), msg.size());
    if (res != msg.size() || check_error()) {
        std::cerr << "Error while sending message!" << '\n';
        active_connection_ = false;
        sockfd_ = close(sockfd_);
    } else {
        std::cout << "Message has been sent successfully!" << '\n';
    }
}

void SocketClient::socket_init() {
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

void SocketClient::connect_to_sock() {
    active_connection_ = connect(sockfd_, reinterpret_cast<sockaddr *>(&server_addr_), sizeof(server_addr_)) >= 0;
}

bool SocketClient::check_error() {
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

bool SocketClient::ensure_connection() {
    if (!active_connection_) {
        if (sockfd_ <= 0) {
            setup_client();
        } else {
            connect_to_sock();
        }
    }
    return active_connection_;
}

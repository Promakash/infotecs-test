#include "handler.h"

void Handler::handle_reponse(const std::vector<char> &buffer, const int data_size) const {
    if (data_size > min_content_size_ && std::atoi(buffer.data()) % divided_by_ == 0) {
        std::cout << "Successfully received data: " << buffer.data() << '\n';
    } else {
        std::cout << "Bad data received!" << '\n';
    }
}

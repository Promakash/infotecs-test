#pragma once

#include <cstring>
#include <vector>
#include <iostream>

class handler {
private:
    const size_t min_content_size_ = 2;
    const size_t divided_by = 32;
public:
    void handle_reponse(const std::vector<char>& buffer) {
        if (buffer.size() > min_content_size_ && std::atoi(buffer.data()) % divided_by == 0) {
            std::cout << "Successfully received data: " << buffer.data() << '\n';
        }
        else {
            std::cout << "Error while receiving data: bad data" << '\n';
        }
    }
};

#pragma once

#include <cstring>
#include <vector>
#include <iostream>

class handler {
public:
    void handle_reponse(const std::vector<char> &buffer, const int data_size) const;

private:
    const size_t min_content_size_ = 2;
    const size_t divided_by = 32;
};

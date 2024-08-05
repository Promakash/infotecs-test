#pragma once

#include <iostream>
#include <algorithm>

class Reader {
public:
    bool validate_str(const std::string &str) const;

    std::string process_str(std::string &str) const;

private:
    const int max_capacity = 64;
    const bool only_numbers = true;
};

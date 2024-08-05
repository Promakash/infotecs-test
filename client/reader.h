#pragma once

#include <iostream>
#include <algorithm>

class Reader {
public:
    const int max_capacity = 64;
    const bool only_numbers = true;

    bool validate_str(const std::string &str) const {
        if (str.size() > max_capacity) {
            return false;
        }
        if (only_numbers) {
            for (const auto &cur_char: str) {
                if (std::isdigit(static_cast<unsigned char>(cur_char)) == false) {
                    std::cerr << "Wrong input data!" << '\n';
                    return false;
                }
            }
        }
        return true;
    }

    std::string process_str(std::string &str) const {
        std::sort(str.begin(), str.end(), std::greater<>());
        std::string processed_str;
        for (const auto &cur_char: str) {
            if (cur_char % 2 == 0) {
                processed_str += "KB";
            } else {
                processed_str += cur_char;
            }
        }
        return processed_str;
    }
};
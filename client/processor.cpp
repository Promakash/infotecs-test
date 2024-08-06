#include "processor.h"

StringProcessor::StringProcessor(std::string &ip, const int port)
    : client_(ip, port), buf_free_(true) {
}

void StringProcessor::Run() {
    std::thread producer_thread(&StringProcessor::run_reader, this);
    std::thread consumer_thread(&StringProcessor::run_processor, this);

    producer_thread.join();
    consumer_thread.join();
}

void StringProcessor::run_reader() {
    std::string str;
    while (std::cin >> str) {
        if (input_reader_.validate_str(str)) {
            str = input_reader_.process_str(str);
            std::cout << "Processed string: " << str << '\n';
            std::unique_lock<std::mutex> lock(mut_);
            buf_free_ = false;
            buffer_ = std::move(str);
            buf_free_ = true;
            buf_used_.notify_one();
        } else {
            std::cout << "Bad input!" << '\n';
        }
    }
}

void StringProcessor::run_processor() {
    client_.setup_client();
    while (true) {
        std::unique_lock<std::mutex> lock(mut_);
        std::string sended_str;
        while (buf_free_ && !buffer_.empty()) {
            buf_used_.wait(lock, [&]() { return buf_free_ || !buffer_.empty(); });
            if (!buffer_.empty()) {
                sended_str = std::move(buffer_);
                buffer_.clear();
            }
        }
        if (!sended_str.empty()) {
            unsigned s = sum_elements(sended_str);
            std::cout << "sum result: " << s << '\n';
            std::string string_sum = std::to_string(s);
            client_.send_msg(string_sum);
        }
    }
}

unsigned StringProcessor::sum_elements(const std::string &str) const {
    unsigned result_sum = 0;
    for (const auto &cur_char: str) {
        if (std::isdigit(static_cast<unsigned char>(cur_char))) {
            result_sum += cur_char - '0';
        }
    }
    return result_sum;
}

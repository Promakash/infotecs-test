#include "processor.h"

StringProcessor::StringProcessor(std::string &ip, const int port)
    : client_(ip, port), buf_free(true) {
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
        if (input_reader.validate_str(str)) {
            str = input_reader.process_str(str);
            std::unique_lock<std::mutex> lock(mut);
            buf_free = false;
            buffer = std::move(str);
            buf_free = true;
            buf_used.notify_one();
        }
    }
}

void StringProcessor::run_processor() {
    client_.setup_client();
    while (true) {
        std::unique_lock<std::mutex> lock(mut);
        std::string sended_str;
        while (buf_free && !buffer.empty()) {
            buf_used.wait(lock, [&]() { return buf_free || !buffer.empty(); });
            if (!buffer.empty()) {
                sended_str = std::move(buffer);
                buffer.clear();
            }
        }
        if (!sended_str.empty()) {
            unsigned s = sum_elements(sended_str);
            std::cout << "sum result: " << sum_elements(sended_str) << '\n';
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
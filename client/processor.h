#pragma once
#include "socketclient.h"
#include "reader.h"
#include "processor.h"
#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>

class StringProcessor {
public:
    StringProcessor(std::string &ip, const int port);

    void Run();

private:
    std::mutex mut_;
    std::condition_variable buf_used_;
    bool buf_free_;
    std::string buffer_;
    Reader input_reader_;
    SocketClient client_;

    unsigned sum_elements(const std::string &str) const;

    void run_reader();

    void run_processor();
};

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
    std::mutex mut;
    std::condition_variable buf_used;
    bool buf_free;
    std::string buffer;
    Reader input_reader;
    SocketClient client_;

    unsigned sum_elements(const std::string &str) const;

    void run_reader();

    void run_processor();
};

#include "sender.h"
#include <algorithm>
#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>


class Reader {
public:
	const int max_capacity = 64;
	const bool only_numbers = true;

	bool validate_str(const std::string& str) const{
		if (str.size() > max_capacity) {
			return false;
		}
		if (only_numbers) {
			for (auto& cur_char : str) {
				if (std::isdigit(static_cast<unsigned char>(cur_char)) == false) {
					return false;
				}
			}
		}
		return true;

	}
	std::string process_str(std::string& str) const{
		std::sort(str.begin(), str.end(), std::greater<>());
		std::string processed_str;
		for (auto& cur_char : str) {
			if (cur_char % 2 == 0) {
				processed_str += "KB";
			}
			else {
				processed_str += cur_char;
			}
		}
		return processed_str;
	}
};

class String_Processor {
public:
	static unsigned sum_elements(const std::string& str){
		std::cout << str << '\n';
		unsigned result_sum = 0;
		for (auto& cur_char : str) {
			if (std::isdigit(static_cast<unsigned char>(cur_char))) {
				result_sum += cur_char - '0';
			}
		}
		return result_sum;
	}
};

class shitdog {
private:
	std::mutex mut;
	std::condition_variable buf_used;
	bool buf_free;
	std::string buffer;
	Reader input_reader;
	String_Processor str_processor;
	Client client_;

public:
	shitdog(std::string &ip, const int port):client_(ip, port) {}

	void run_reader() {
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

	void run_processor() {
		client_.setup_client();
		for(;;) {
			std::unique_lock<std::mutex> lock(mut);
			std::string sended_str;
			while (buf_free && !buffer.empty()) {
				buf_used.wait(lock, [&]() {return buf_free || !buffer.empty(); });
				if (!buffer.empty()) {
					sended_str = std::move(buffer);
					buffer.clear();
				}
			}
			if (!sended_str.empty()) {
				unsigned s = str_processor.sum_elements(sended_str);
				std::cout << "sum result: " << str_processor.sum_elements(sended_str);
				std::string string_sum = std::to_string(s);
				client_.send_msg(string_sum);
			}
		}
	}

	void Run() {
		std::thread producer_thread(&shitdog::run_reader, this);
		std::thread consumer_thread(&shitdog::run_processor, this);

		producer_thread.join();
		consumer_thread.join();
	}
};

int main() {
	std::string ip = "127.0.0.1";
	const int port = 8000;
	shitdog serv(ip, port);

	serv.Run();

	return 0;
}
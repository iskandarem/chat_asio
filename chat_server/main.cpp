#include <iostream>
#include "server.hpp"
#include <boost/asio.hpp>
#include <string>
#include <thread>
#include <format>

using namespace std::chrono_literals;

void testing_async() {
	auto thread_id_to_string = [](const auto& id) -> std::string {
		std::stringstream ss;
		ss << id;
		return ss.str();
	};
	auto heavy_task = [&thread_id_to_string]() -> void {
		std::cout << std::format("Heavy task running from thread {}...\n", thread_id_to_string(std::this_thread::get_id()));
		std::this_thread::sleep_for(5s);
		std::cout << "Heavy task finished\n";
	};
	auto light_task = [&thread_id_to_string]() -> void {
		std::cout << std::format("Light task running from thread {}.\n", thread_id_to_string(std::this_thread::get_id()));
	};
	boost::asio::thread_pool io{2};
	boost::asio::post(io, heavy_task);	
	boost::asio::post(io, light_task);
	io.join();
}
		
int main(int argc, const char** argv)
{
	try {
		if(argc < 3) {
			std::cout << "Usage: chat_server host port\n";
			return -1;
		}
		std::string host{argv[1]};
		int port = std::stoi(std::string{argv[2]});
		asio::io_context io;
		Server server{io, host, port};
		
		std::cout << std::format("Server listening on {}:{}\n", host, port);
		
		io.run();
	}
	catch(std::exception& e) {
		std::cerr << std::format("Error: {}\n", e.what());
	}
	return 0;	
}

#include <iostream>
#include <boost/asio.hpp>
#include <boost/program_options.hpp>
#include <functional>
#include <thread>
#include "mem_func_comp_handler.hpp"

namespace asio = boost::asio;
namespace po   = boost::program_options;


//void print(const boost::system::error_code& e, asio::steady_timer* t, int& count) {
//	if(count < 5) {
//		std::cout << count << '\n';
//		++count;
//		t->expires_at(t->expiry() + asio::chrono::seconds(1));
//		t->async_wait(std::bind(print,
//			asio::placeholders::error, t, count));
//	}
//}

int main(int argc, const char** argv)
{
// 	asio::io_context io;
// 	int count{};
// 	asio::steady_timer t{io, asio::chrono::seconds(1)};
// 	t.async_wait(std::bind(print,
// 		asio::placeholders::error, &t, count));
// 	io.run();
// 	std::cout << "Final count is " << count << std::endl;
//	asio::io_context io;
//	printer p{io};
//	printer p1{io};
//	io.run();
	asio::io_context io;
	printer p{io};
	std::jthread t([&]{io.run();});
	io.run();
	return 0;
}

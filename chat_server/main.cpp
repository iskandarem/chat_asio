#include <iostream>
#include <boost/asio.hpp>
#include <boost/program_options.hpp>

namespace asio = boost::asio;
namespace po   = boost::program_options;

int main(int argc, const char** argv)
{
	asio::io_context io;
	asio::steady_timer t{io, asio::chrono::seconds(5)};
	auto pred = [](const boost::system::error_code& error){
		std::cout << "Hello, User!\n";
	};

	t.async_wait(pred);
	io.run();
}

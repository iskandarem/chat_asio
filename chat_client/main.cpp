#include <iostream>
#include <string>
#include <format>
#include <boost/asio.hpp>

namespace asio = boost::asio;
using tcp  = boost::asio::ip::tcp;

int main(int argc, char* argv[]) {
	try {
		asio::io_context io;
		tcp::socket socket{io};
	
		// connecting to the server thast is accessible at 127.0.0.1:8000	
		socket.connect(
			tcp::endpoint(
				asio::ip::make_address("0.0.0.0"),
				8000
			)
		);

		std::cout << "Connected to the server!\n";
		
		std::string message = "Hello world!\n";
		
		asio::write(	
			socket, 
			asio::buffer(message)
		);

		asio::streambuf response_buffer;
		
		asio::read_until( 
			socket,
			response_buffer,
			'\n'
		);
		
		std::istream response{&response_buffer};

		std::string line;
		std::getline(response, line);
		
		std::cout << std::format("Server: {}\n", line);	
	}
	catch(std::exception& e) {
		std::cout << std::format("Error: {}", e.what());
	}
	return 0;
}

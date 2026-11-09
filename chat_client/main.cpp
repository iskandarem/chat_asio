#include <cstdlib>
#include <iostream>
#include <string>

#include <boost/asio.hpp>

#include "client.hpp"

namespace asio = boost::asio;

int main(int argc, char* argv[]) {
	if (argc != 3) {
		std::cerr << "Usage: chat_client host port\n";
		return EXIT_FAILURE;
	}

	try {
		asio::io_context io;
		std::string msg;
		while(!msg.starts_with(">exit")) {
			Client client{io, argv[1], argv[2]};

			if (!std::getline(std::cin, msg)) {
				std::cerr << "Error: failed to read a message from standard input\n";
				return EXIT_FAILURE;
			}

			std::cout << "Sending '" << msg << "' to the server\n";
			if (!client.send(msg)) {
				return EXIT_FAILURE;
			}
		}
	}
	catch (const std::exception& e) {
		std::cerr << "Error: " << e.what() << '\n';
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}

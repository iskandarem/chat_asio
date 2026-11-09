#include "client.hpp"

#include <iostream>
#include <string>

Client::Client(asio::io_context& io, std::string_view host, std::string_view port)
	: socket_{io} {
	tcp::resolver resolver{io};
	const auto endpoints = resolver.resolve(std::string{host}, std::string{port});
	asio::connect(socket_, endpoints);

	std::cout << "Connected to server\n";
}

bool Client::send(std::string_view msg) {
	try {
		std::string request{msg};
		request.push_back('\n');
		asio::write(socket_, asio::buffer(request));

		asio::streambuf response;
		asio::read_until(socket_, response, '\n');

		std::istream input{&response};
		std::string line;
		std::getline(input, line);
		std::cout << line << '\n';
	}
	catch (const std::exception& e) {
		std::cerr << "Error: " << e.what() << '\n';
		return false;
	}

	return true;
}

#include "server.hpp"

#include <iostream>

Server::Server(asio::io_context& io, const std::string& address, int port)
	: acceptor_{
		io,
		tcp::endpoint(
			asio::ip::make_address(address),
			port
		)
	} {
	accept_next();
}

void Server::accept_next() {
	if(!acceptor_.is_open()) {
		return;
	}

	acceptor_.async_accept(
		[this](const boost::system::error_code& ec,
				tcp::socket socket) {
			if(!ec) {
				std::cout << "Client connected!\n";
				
				std::make_shared<Session>(
					std::move(socket)
				)->start();
			} else if(ec == asio::error::operation_aborted) {
				return;
			} else {
				std::cerr << "Accept failed: " << ec.message() << '\n';
			}

			if(acceptor_.is_open()) {
				accept_next();
			}
		}
	);
}
		

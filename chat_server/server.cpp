#include "server.hpp"

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
	acceptor_.async_accept(
		[this](const boost::system::error_code& ec,
				tcp::socket socket) {
			if(!ec) {
				std::cout << "Client connected!\n";
				
				std::make_shared<Session>(
					std::move(socket)
				)->start();
			}
			
			accept_next();
		}
	);
}
		

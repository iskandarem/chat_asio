#pragma once 
#include "session.hpp"

struct Server {
	explicit Server(asio::io_context& io, const std::string& address, int port);

private:
	void accept_next();
	
	// Data
	tcp::acceptor acceptor_;
};

#pragma once 
#include <boost/asio.hpp>
#include <memory>
#include <string>
#include <utility>
#include <iostream>

namespace asio = boost::asio;
using tcp = asio::ip::tcp;
struct Session
	: std::enable_shared_from_this<Session> {
	explicit Session(tcp::socket socket);
	void start();
private:
	void read_line();
	void write_line();
	
	// Data
	tcp::socket socket_;
	asio::streambuf input_;
	std::string output_;
};


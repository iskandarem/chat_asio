#pragma once 
#include <boost/asio.hpp>
#include <cstddef>
#include <memory>
#include <string>
#include <utility>

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
	static constexpr std::size_t max_input_size_ = 4096;

	tcp::socket socket_;
	asio::streambuf input_{max_input_size_};
	std::string output_;
};


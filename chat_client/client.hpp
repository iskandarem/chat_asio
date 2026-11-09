#pragma once

#include <boost/asio.hpp>

#include <string_view>

namespace asio = boost::asio;
using tcp =	asio::ip::tcp;

struct Client {
	explicit Client(asio::io_context& io, std::string_view host, std::string_view port);
	bool send(std::string_view msg);

private:
	tcp::socket socket_;
};

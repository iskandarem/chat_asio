#include "session.hpp"

#include <iostream>
#include <istream>

Session::Session(tcp::socket socket)
	: socket_{std::move(socket)} { }

void Session::start() {
	read_line();
}

void Session::read_line() {
	auto self = shared_from_this();
	
	asio::async_read_until(
		socket_,
		input_,
		'\n',
		
		[self](const boost::system::error_code& ec, std::size_t) {
			if(ec) {
				if(ec == asio::error::not_found) {
					std::cerr << "Client message exceeds the "
						<< max_input_size_ - 1
						<< " byte limit or has no newline\n";
				} else if(ec != asio::error::eof
						&& ec != asio::error::operation_aborted) {
					std::cerr << "Session read failed: " << ec.message() << '\n';
				}
				return;
			}

			std::istream stream{&self->input_};
			std::string line;
			if(!std::getline(stream, line)) {
				std::cerr << "Session received an invalid line\n";
				return;
			}

			if(!line.empty() && line.back() == '\r') {
				line.pop_back();
			}

			self->output_ = "echo: " + line + '\n';

			self->write_line();
		}
	);
}

void Session::write_line() {
	auto self = shared_from_this();
	
	asio::async_write(
		socket_,
		asio::buffer(output_),
		
		[self](const boost::system::error_code& ec, std::size_t) {
			if(ec) {
				if(ec != asio::error::operation_aborted) {
					std::cerr << "Session write failed: " << ec.message() << '\n';
				}
				return;
			}

			self->read_line();
		}
	);
}	

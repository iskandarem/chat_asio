#include "session.hpp"

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
			if(ec) return ;

			std::istream stream{&self->input_};
			
			std::string line;
			std::getline(stream, line);

			self->output_ = "echo: " + line + '\n';

			self-> write_line();
		}
	);
}

void Session::write_line() {
	auto self = shared_from_this();
	
	asio::async_write(
		socket_,
		asio::buffer(output_),
		
		[self](const boost::system::error_code& ec, std::size_t) {
			if(!ec) {
				self->read_line();
			}
		}
	);
}	

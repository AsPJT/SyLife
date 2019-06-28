#include <iostream>
#include <boost/asio.hpp>

namespace asio = boost::asio;
using asio::ip::tcp;

int main()
{
	asio::io_service io_service;
	tcp::socket socket(io_service);

	// �ڑ�
	socket.connect(tcp::endpoint(asio::ip::address::from_string("127.0.0.1"), 31400));

	// ���b�Z�[�W���M
	const std::string msg = "ping";
	boost::system::error_code error;
	asio::write(socket, asio::buffer(msg), error);

	if (error) {
		std::cout << "send failed: " << error.message() << std::endl;
	}
	else {
		std::cout << "send correct!" << std::endl;
	}
}
#ifdef _WIN32
#define _WIN32_WINNT 0x0A00
#endif
#define ASIO_STANDALONE

#include <iostream>
#include <string>
#include <thread>

#include <asio.hpp>
#include <asio/ts/buffer.hpp>
#include <asio/ts/internet.hpp>


std::vector<char> buffer(20 * 1024);

void ReadData(asio::ip::tcp::socket& socket) {
	socket.async_read_some(asio::buffer(buffer.data(), buffer.size()), 
	[&](std::error_code ec, std::size_t length) {
		if(!ec) {
			for(int i = 0; i < length; i++)
				std::cout << buffer[i];

			ReadData(socket);
		}
	});
}

int main(){
	asio::error_code ec;

	asio::io_context context;

	// idle work so asio does not exit.
	asio::io_context::work idleWork(context);

	// Run asio context in own thread.
	std::thread contextThread = std::thread([&](){ context.run(); });

	// Connect to endpoint over socket.
	asio::ip::tcp::endpoint endpoint(asio::ip::make_address("93.184.216.34", ec), 80);
	asio::ip::tcp::socket socket(context);
	socket.connect(endpoint, ec);

	if(socket.is_open()) {
		ReadData(socket);

		std::string request = \
			"GET /index.html HTTP/1.1\r\n" \
			"Host: example.com\r\n" \
			"Connection: close\r\n\r\n";

		socket.write_some(asio::buffer(request.data(), request.size()), ec);

		using namespace std::chrono_literals;
		std::this_thread::sleep_for(2000ms);

		context.stop();
		if(contextThread.joinable())
			contextThread.join();
	}

	return 0;
}

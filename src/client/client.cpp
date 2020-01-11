#include "message/message.h"
#include "message/transfer.h"

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include <cstdlib>
#include <iostream>
#include <string>
#include <thread>

void process_keyboard_input_loop(int connection_fd) {
	std::cout << "> " << std::flush;
	while (true) {
		std::string line;
		std::getline(std::cin, line);

		Message message(line);
		send_message(connection_fd, message);
	}
}

void connection_recv_loop(int connection_fd) {
	while (true) {
		Message m = recv_message(connection_fd);
		std::cout << "\nReceived message: " << m.get_line() << std::endl;
		std::cout << "> " << std::flush;
	}
}

int main(int argc, char** argv) {
	if (argc > 2) {
		std::cerr << "chat-client [ip-address]" << std::endl;
		exit(EXIT_FAILURE);
	}

	const char* ip_address = "127.0.0.1";
	if (argc == 2) {
		ip_address = argv[1];
	}

	sockaddr_in server_address;
	server_address.sin_family = AF_INET;
	server_address.sin_port = htons(5000);

	if (inet_pton(AF_INET, ip_address, &server_address.sin_addr.s_addr) != 1) {
		std::cerr << "Invalid IP address" << std::endl;
		exit(EXIT_FAILURE);
	}

	int connection_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (connection_fd == -1) {
		std::cerr << "Socket creation failed" << std::endl;
		exit(EXIT_FAILURE);
	}

	
	if (connect(connection_fd, (sockaddr*)&server_address, sizeof(server_address)) == -1) {
		std::cerr << "Connect failed" << std::endl;
		exit(EXIT_FAILURE);
	}

	std::thread t(connection_recv_loop, connection_fd);
	t.detach();

	process_keyboard_input_loop(connection_fd);

	if (close(connection_fd) == -1) {
		std::cerr << "Close failed" << std::endl;
		exit(EXIT_FAILURE);
	}

	return 0;
}
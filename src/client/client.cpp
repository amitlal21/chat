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
	while (true) {
		std::cout << "> ";
		std::string line;
		std::getline(std::cin, line);

		Message message(line);
		send_message(connection_fd, message);

	}
}

void connection_recv_loop(int connection_fd) {
	while (true) {
		Message m = recv_message(connection_fd);
		std::cout << "Received message: " << m.get_line() << std::endl;
	}
}

int main(int, char**) {
	int connection_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (connection_fd == -1) {
		std::cerr << "Socket creation failed" << std::endl;
		exit(EXIT_FAILURE);
	}

	sockaddr_in server_address;
	server_address.sin_family = AF_INET;
	server_address.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
	server_address.sin_port = htons(5000);
	if (connect(connection_fd, (struct sockaddr*)&server_address, sizeof(server_address)) == -1) {
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
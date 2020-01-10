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

void process_keyboard_input_loop(int socket_fd) {
	while(true) {
		std::cout << "> ";
		std::string line;
		std::getline(std::cin, line);

		Message message(line);
		send_message(socket_fd, message);

		Message m = recv_message(socket_fd);
		std::cout << "Server message: " << m.get_line() << std::endl;
	}
}

int main(int, char**) {
	int socket_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (socket_fd == -1) {
		std::cerr << "Socket creation failed" << std::endl;
		exit(EXIT_FAILURE);
	}

	sockaddr_in server_address;
	server_address.sin_family = AF_INET;
	server_address.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
	server_address.sin_port = htons(5000);
	if(connect(socket_fd, (struct sockaddr*)&server_address, sizeof(server_address)) == -1) {
		std::cerr << "Connect failed" << std::endl;
		exit(EXIT_FAILURE);
	}

	process_keyboard_input_loop(socket_fd);

	if(close(socket_fd) == -1) {
		std::cerr << "Close failed" << std::endl;
		exit(EXIT_FAILURE);
	}

	return 0;
}
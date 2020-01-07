#include "message/Message.h"

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include <cstdlib>
#include <iostream>
#include <string>

void send_message(int socket_fd, const Message& message) {
	const char* ptr = reinterpret_cast<const char*>(&message);
	const char* end_ptr = ptr + message.size();

	while (ptr != end_ptr) {
		int sent = send(socket_fd, ptr, end_ptr - ptr, 0);
		if (sent == -1) {
			std::cerr << "Send failed" << std::endl;
			exit(EXIT_FAILURE);
		}
		ptr += sent;
	}
	
}

void process_keyboard_input_loop(int socket_fd) {
	while(true) {
		std::cout << "> ";
		std::string line;
		std::getline(std::cin, line);

		Message message(line);
		send_message(socket_fd, message);

		std::cout << line << std::endl;
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
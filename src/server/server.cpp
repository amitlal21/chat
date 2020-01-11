#include "state.h"
#include "message/message.h"
#include "message/transfer.h"

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include <cstdlib>
#include <exception>
#include <iostream>
#include <string>
#include <thread>

void connection_recv_loop(int connection_fd, sockaddr_in client_address) {
	std::cout << "Client connected: " << inet_ntoa(client_address.sin_addr) << ":" << ntohs(client_address.sin_port) << std::endl;
	
	State::get_instance().add_connection(connection_fd);
	try {
		while (true) {
			Message m = recv_message(connection_fd);
			std::cout << "Client message: " << m.get_line() << std::endl;
			State::get_instance().broadcast_message(m);
		}
	} catch (const std::exception& e) {
		std::cerr << e.what() << std::endl;
	}
	State::get_instance().remove_connection(connection_fd);
	
	std::cout<< "Client disconnected: " << inet_ntoa(client_address.sin_addr) << ":" << ntohs(client_address.sin_port) << std::endl;

	if (close(connection_fd) == -1) {
		std::cerr << "Close failed: fd = " << connection_fd << std::endl;
	}
}

void handle_new_connection(int connection_fd, const sockaddr_in& client_address) {	
	std::thread	t(connection_recv_loop, connection_fd, client_address);
	t.detach();
}

void process_listening_socket(int socket_fd) {
	while (true) {
		sockaddr_in client_address;
		socklen_t client_address_len = sizeof(client_address);
		int connection_fd = accept(socket_fd, (struct sockaddr*)&client_address, &client_address_len);
		if (connection_fd == -1) {
			std::cerr << "Accept failed" << std::endl;
			exit(EXIT_FAILURE);
		}

		handle_new_connection(connection_fd, client_address);
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
	server_address.sin_addr.s_addr = htonl(INADDR_ANY);
	server_address.sin_port = htons(5000);
	if (bind(socket_fd, (struct sockaddr*)&server_address, sizeof(server_address)) == -1) {
		std::cerr << "Bind failed" << std::endl;
		exit(EXIT_FAILURE);
	}

	const int backlog = 5;
	if (listen(socket_fd, backlog) == -1) {
		std::cerr << "Listen failed" << std::endl;
		exit(EXIT_FAILURE);
	}

	process_listening_socket(socket_fd);
	

	if (close(socket_fd) == -1) {
		std::cerr << "Close failed" << std::endl;
		exit(EXIT_FAILURE);
	}


	return 0;
}
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include <cstdlib>
#include <iostream>
#include <string>

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

	sockaddr_in client_address;
	socklen_t client_address_len = sizeof(client_address);
	int newsocket_fd = accept(socket_fd, (struct sockaddr*)&client_address, &client_address_len);
	if (newsocket_fd == -1) {
		std::cerr << "Accept failed" << std::endl;
		exit(EXIT_FAILURE);
	}

	std::cout << "Client connected: " << inet_ntoa(client_address.sin_addr) << ":" << ntohs(client_address.sin_port) << std::endl;

	if(close(socket_fd) == -1) {
		std::cerr << "Close failed" << std::endl;
		exit(EXIT_FAILURE);
	}

	if(close(newsocket_fd) == -1) {
		std::cerr << "Close failed" << std::endl;
		exit(EXIT_FAILURE);
	}
	return 0;
}
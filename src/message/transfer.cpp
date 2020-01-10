#include "transfer.h"

#include <sys/socket.h>
#include <sys/types.h>

#include <stdexcept>

void send_message(int socket_fd, const Message& message) {
	const char* ptr = reinterpret_cast<const char*>(&message);
	const char* end_ptr = ptr + message.size();

	while (ptr != end_ptr) {
		int sent = send(socket_fd, ptr, end_ptr - ptr, 0);
		if (sent == -1) {
			throw std::runtime_error("Send failed");
		}
		ptr += sent;
	}
}

void recv_bytes(int socket_fd, char*& ptr, char* ptr_end) {
	while (ptr != ptr_end) {
		int received = recv(socket_fd, ptr, ptr_end - ptr, 0);
		if (received == -1) {
			throw std::runtime_error("Receive failed");
		}
		if (received == 0) {
			throw std::runtime_error("Peer shutdown");
		}
		ptr += received;
	}
}

Message recv_message(int socket_fd) {
	Message m;
	char* ptr = reinterpret_cast<char*>(&m);

	char* ptr_end = ptr + Message::HEADER_SIZE;
	recv_bytes(socket_fd, ptr, ptr_end);

	if (!m.is_valid()) {
		throw std::runtime_error("Invalid message");
	}

	ptr_end += m.size() - Message::HEADER_SIZE;
	recv_bytes(socket_fd, ptr, ptr_end);

	return m;
}
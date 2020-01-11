#include "state.h"
#include "message/transfer.h"

State::State() {
}

State& State::get_instance() {
	static State state;
	return state;
}

void State::add_connection(int fd) {
	std::lock_guard<std::mutex> lock(mutex);
	connections.insert(fd);
}

void State::remove_connection(int fd) {
	std::lock_guard<std::mutex> lock(mutex);
	connections.erase(fd);
}

void State::broadcast_message(const Message& message) const {
	std::lock_guard<std::mutex> lock(mutex);
	for (int fd : connections) {
		send_message(fd, message);
	}
}
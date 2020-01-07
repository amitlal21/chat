#ifndef MESSAGE_MESSAGE_H
#define MESSAGE_MESSAGE_H

#include <cstdint>
#include <stdexcept>
#include <string>

class Message {
public:
	static const int MAX_MESSAGE_SIZE = 255;

	Message(const std::string& message) {
		if (message.size() > MAX_MESSAGE_SIZE) {
			throw std::invalid_argument("Message exceeds 255 characters");
		}
		version = 0;
		message_size = message.size();
		message.copy(message_data, message_size);
	}

	std::size_t size() const {
		return sizeof(version) + sizeof(message_size) + message_size;
	}

private:
	std::int8_t version;
	std::int8_t message_size;
	char message_data[MAX_MESSAGE_SIZE];
};

#endif
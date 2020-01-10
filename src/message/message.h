#ifndef MESSAGE_MESSAGE_H
#define MESSAGE_MESSAGE_H

#include "header.h"

#include <cstdint>
#include <stdexcept>
#include <string>

class Message {
public:
	static const int HEADER_SIZE = sizeof(Header);
	static const int MAX_LINE_SIZE = (1 << (sizeof(Header::body_size) * 8)) - 1;

	Message() {
	}

	Message(const std::string& line) {
		if (line.size() > MAX_LINE_SIZE) {
			throw std::invalid_argument("Message exceeds 255 characters");
		}
		header.body_size = line.size();
		line.copy(line_data, header.body_size);
	}

	std::size_t size() const {
		return HEADER_SIZE + header.body_size;
	}

	std::string get_line() const {
		return std::string(line_data, header.body_size);
	}

	bool is_valid() const {
		return header.is_valid();
	}

private:
	Header header;
	char line_data[MAX_LINE_SIZE];
};

#endif
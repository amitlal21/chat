#ifndef MESSAGE_HEADER_H
#define MESSAGE_HEADER_H

#include <cstdint>

struct Header {
	static const std::uint8_t CURRENT_VERSION = 0;

	Header() {
		version = CURRENT_VERSION;
		body_size = 0;
	}

	bool is_valid() const {
		return version == CURRENT_VERSION;
	}

	std::uint8_t version;
	std::uint8_t body_size;
};

#endif
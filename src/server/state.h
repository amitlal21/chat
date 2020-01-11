#ifndef SERVER_STATE_H
#define SERVER_STATE_H

#include "message/message.h"

#include <mutex>
#include <unordered_set>

class State {
public:
	static State& get_instance();

	void add_connection(int fd);

	void remove_connection(int fd);

	void broadcast_message(const Message& message) const;

private: 
	State();

	mutable std::mutex mutex;
	
	std::unordered_set<int> connections;
};

#endif
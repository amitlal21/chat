CXXFLAGS := -Wall -Wextra -Wpedantic -Werror
CXXFLAGS += -std=c++14 -pthread
CXXFLAGS += -I./src

.PHONY: all
all: bin/chat-client bin/chat-server

.PHONY: clean
clean: | bin
	rm -f bin/chat-client bin/chat-server
	rmdir bin

common_prereqs := src/constants/port.h
common_prereqs += src/message/header.h
common_prereqs += src/message/message.h
common_prereqs += src/message/transfer.h
common_prereqs += src/message/transfer.cpp
common_prereqs += Makefile

client_prereqs := src/client/client.cpp
client_prereqs += $(common_prereqs)

server_prereqs := src/server/server.cpp
server_prereqs += src/server/state.h
server_prereqs += src/server/state.cpp
server_prereqs += $(common_prereqs)

bin/chat-client: $(client_prereqs) | bin
	g++ $(CXXFLAGS) -o $@ $(filter %.cpp,$^)

bin/chat-server: $(server_prereqs) | bin
	g++ $(CXXFLAGS) -o $@ $(filter %.cpp,$^)

bin:
	mkdir $@
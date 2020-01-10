CXXFLAGS := -Wall -Wextra -Wpedantic -Werror
CXXFLAGS += -std=c++14
CXXFLAGS += -I./src

.PHONY: all
all: bin/chat-client bin/chat-server

.PHONY: clean
clean: | bin
	rm -f bin/chat-client bin/chat-server
	rmdir bin

common_prereqs := src/message/header.h
common_prereqs += src/message/message.h
common_prereqs += src/message/transfer.h
common_prereqs += src/message/transfer.cpp
common_prereqs += Makefile

bin/chat-client: src/client/client.cpp $(common_prereqs) | bin
	g++ $(CXXFLAGS) -o $@ $(filter %.cpp,$^)

bin/chat-server: src/server/server.cpp $(common_prereqs) | bin
	g++ $(CXXFLAGS) -o $@ $(filter %.cpp,$^)

bin:
	mkdir $@
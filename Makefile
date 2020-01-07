CXXFLAGS := -Wall -Wextra -Wpedantic -Werror
CXXFLAGS += -std=c++14
CXXFLAGS += -I./src

.PHONY: all
all: bin/chat-client bin/chat-server

.PHONY: clean
clean: | bin
	rm -f bin/chat-client bin/chat-server
	rmdir bin

bin/chat-client: src/client/client.cpp src/message/Message.h Makefile | bin
	g++ $(CXXFLAGS) -o $@ $(filter %.cpp,$^)

bin/chat-server: src/server/server.cpp Makefile | bin
	g++ $(CXXFLAGS) -o $@ $(filter %.cpp,$^)

bin:
	mkdir $@
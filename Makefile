CXXFLAGS := -Wall -Wextra -Wpedantic -Werror

.PHONY: all
all: bin/chat-client bin/chat-server

.PHONY: clean
clean: | bin
	rm -f bin/chat-client bin/chat-server
	rmdir bin

bin/chat-client: src/client/client.cpp Makefile | bin
	g++ $(CXXFLAGS) -o $@ $(filter-out Makefile,$^)

bin/chat-server: src/server/server.cpp Makefile | bin
	g++ $(CXXFLAGS) -o $@ $(filter-out Makefile,$^)

bin:
	mkdir $@
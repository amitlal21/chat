bin/chat-client: src/client/client.cpp | bin
	g++ -o $@ $^

bin:
	mkdir $@
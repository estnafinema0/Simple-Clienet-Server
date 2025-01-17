all: client server
CXXFLAGS+=-std=c++20

client: client.cpp network.o
server_thread: server.cpp network.o
network.o: network.hpp

clean:
	rm -f client server *.o

.PHONY: all clean
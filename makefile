CC=g++

compile: server client
server : server.o
	$(CC) -o server server.o -lpthread
client : client.o
	$(CC) -o client client.o
server.o: server.cpp
	$(CC) -c server.cpp
client.o: client.cpp
	$(CC) -c client.cpp
clean:
	rm -rf *.o
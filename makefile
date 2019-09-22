CC=g++

compile: server client
server : server.o messagepassing.o
	$(CC) -o server server.o messagepassing.o -lpthread
client : client.o messagepassing.o
	$(CC) -o client client.o messagepassing.o
server.o: server.cpp
	$(CC) -c server.cpp
client.o: client.cpp
	$(CC) -c client.cpp
messagepassing.o: messagepassing.cpp
	$(CC) -c messagepassing.cpp
clean:
	rm -rf *.o
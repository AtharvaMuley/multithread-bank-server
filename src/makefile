CC=g++

compile: server client client-multithread
server : server.o messagepassing.o bank_account.o
	$(info )
	$(info ******** Compiling Server program ********)
	$(CC) -o server server.o messagepassing.o bank_account.o  -lpthread
client : client.o messagepassing.o
	$(info )
	$(info ******** Compiling Cient program ********)
	$(CC) -o client client.o messagepassing.o
client-multithread: client-multithread.o messagepassing.o
	$(info )
	$(info ******** Compiling Cient-Multithread program ********)
	$(CC) -o  client-multithread client-multithread.o messagepassing.o -lpthread
server.o: server.cpp
	$(CC) -c server.cpp
client.o: client.cpp
	$(CC) -c client.cpp
client-multithread.o: client-multithread.cpp
	$(CC) -c client-multithread.cpp
messagepassing.o: messagepassing.cpp
	$(CC) -c messagepassing.cpp
bank_account.o: bank_account.cpp
	$(CC) -c bank_account.cpp
clean:
	rm -rf *.o client server client-multithread

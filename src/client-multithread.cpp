#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <fstream>
#include "messagepassing.h"
#include <pthread.h>

#define NO_OF_THREADS 30
int counter = 0;
pthread_mutex_t filemutex;
pthread_cond_t filecond;
std::ofstream filefd;

/*
* writeToFile logs the data passed to it as a parameter.
* the thread must first hold the mutex to write into file
* to avoid the race condition.
*/
void writeToFile(char *tran, char *msg){
	pthread_mutex_lock(&filemutex);
	while(!&filecond){
		pthread_cond_wait(&filecond, &filemutex);
	}
	filefd << tran<< " -> " << msg << std::endl;
	filefd <<"+++++++++++++++++++++++++++++++++++++++++++++++++++\n";
	pthread_cond_signal(&filecond);
	pthread_mutex_unlock(&filemutex);
}

/*
* clientWorker tries to connect to the server.
* Upon successfull connection send the transaction to the server
* then print the transaction response on the console and log it to
* the file.
*/
void *clientWorker(void* args){
	char *mmg;
	char *servMsg;
	mmg = (char *) args;
	int clientfd,clientlen;
	struct sockaddr_in servaddr;
	clientfd = socket(AF_INET,SOCK_STREAM,0);
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	servaddr.sin_port = htons(8090);

	clientlen = sizeof(servaddr);
	connect(clientfd,(struct sockaddr *)&servaddr,clientlen);

	MessagePassing message(clientfd);
	
	//Send the transactions to the server
	message.sendMessage(mmg);
	
	char *msg = message.receiveMessage();
	std::cout<< mmg<< " : " << msg << std::endl;
	//Log the transaction status
	writeToFile(mmg,msg);
	
	close(clientfd);
	counter--;
	pthread_exit(NULL);
}

int main(int argc, char *argv[]){
	filefd.open("Logs-Multithread.txt",std::ios::app);
	pthread_t threads[NO_OF_THREADS];
	std::string line;
	std::ifstream fileread("Transactions.txt");
	if (fileread.is_open()){
		/*
		* Read each transaction from Transactions.txt and copy it in line variable
		*/
		while(getline(fileread, line)){
			char servMsg;
			//For evey trasaction in Transactions.txt spawn a new thread
			pthread_create(&threads[counter],NULL, clientWorker,(void*)line.c_str());
			pthread_join(threads[counter],NULL);
			counter++;
			while (counter > NO_OF_THREADS){
        			/* While thread limit had reached wait till the current clients close the connection*/
        		}
		}
	}
	else{
		std::cout << "Unable to open Transactions.txt" << std::endl;
		std::_Exit(EXIT_FAILURE);
	}
	return 1;
}

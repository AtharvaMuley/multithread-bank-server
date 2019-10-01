#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <fstream>
#include "messagepassing.h"
#include <pthread.h>

int counter = 0;

//list<char*> fileQueue;
pthread_mutex_t filemutex;
pthread_cond_t filecond;
std::ofstream filefd;
void writeToFile(char *tran, char *msg){
	//std::cout<<"Generating file\n";
	pthread_mutex_lock(&filemutex);
	while(!&filecond){
	pthread_cond_wait(&filecond, &filemutex);
	}
	filefd << tran<< " -> " << msg << std::endl;
	filefd <<"+++++++++++++++++++++++++++++++++++++++++++++++++++\n";
	pthread_cond_signal(&filecond);
	pthread_mutex_unlock(&filemutex);
}

void *clientWorker(void* args){
char *mmg;
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

//Send message to server
char *servMsg;
//strcpy(servMsg, line.c_str());
message.sendMessage(mmg);
//std::cout << servMsg <<std::endl;
//Receive from server
char *msg = message.receiveMessage();
std::cout<< mmg<< " : " << msg << std::endl;
writeToFile(mmg,msg);
//shutdown(clientfd,SHUT_RDWR);
close(clientfd);
counter--;
pthread_exit(NULL);
}

int main(int argc, char *argv[]){
filefd.open("Logs-Multithread.txt",std::ios::app);
pthread_t threads[100];
std::string line;
std::ifstream fileread("Transactions.txt");
if (fileread.is_open()){
	
	while(getline(fileread, line)){
		char servMsg;
		pthread_create(&threads[counter],NULL, clientWorker,(void*)line.c_str());
		pthread_join(threads[counter],NULL);
		counter++;
	}
}
else{
	std::cout << "Unable to open Transactions.txt" << std::endl;
std::_Exit(EXIT_FAILURE);
}
    return 1;
}

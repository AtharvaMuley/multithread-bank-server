#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <fstream>
#include "messagepassing.h"

int main(int argc, char *argv[]){
    		
	std::string line;
	std::ifstream fileread("Transactions.txt");
	if (fileread.is_open()){
		
		while(getline(fileread, line)){
			std::cout << line <<std::endl;
			
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
			//line.copy(message, line.length(), 0);
			message.sendMessage("101 101 d 500");

			//Receive from server
			std::string msg = message.receiveMessage();
			std::cout<< msg << std::endl;
			shutdown(clientfd,SHUT_RDWR);
			close(clientfd);
			
		}
	}
	else{
		std::cout << "Unable to open Transactions.txt" << std::endl;
        std::_Exit(EXIT_FAILURE);
	}

    
    sleep(3);
    //Send message to server
    //message.sendMessage("101 104 d 500");
    //Receive from server
    //std::string msg2 = message.receiveMessage();
    //std::cout<< msg2 << std::endl;
    return 1;
}

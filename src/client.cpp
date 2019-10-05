#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <fstream>
#include "messagepassing.h"

int main(int argc, char *argv[]){	
	std::string line;
	//Create Transactions.txt
	std::ifstream fileread("Transactions.txt");
	//create Logs-SingleThread.txt
	std::ofstream logwrite("Logs-SingleThread");

	if (fileread.is_open()){
		if(logwrite.is_open()){
			/*
			* Read each transaction from Transactions.txt and copy it in line variable
			*/
			while(getline(fileread, line)){
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
				
				char servMsg[1024] = {0};
				strcpy(servMsg, line.c_str());
				//Send the transaction to the server
				message.sendMessage(servMsg);
				
				//Receive the transaction status from server
				char *msg = message.receiveMessage();
				std::cout<< line << " -> " << msg<< std::endl;
				logwrite << line << " -> " << msg << "\n+++++++++++++++++++++++++++++++++++++++++++++++++++++\n";
				//close the client
				close(clientfd);
			}
		}
		else{
			std::cout << "Error opening Log SingleThread file\nExiting...";
			return 0;
		}	
	}
	else{
		std::cout << "Unable to open Transactions.txt" << std::endl;
        	std::_Exit(EXIT_FAILURE);
	}
    return 1;
}

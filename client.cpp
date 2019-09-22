#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "messagepassing.h"

int main(int argc, char *argv[]){
    int clientfd,clientlen;
    struct sockaddr_in servaddr;

    clientfd = socket(AF_INET,SOCK_STREAM,0);

    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    servaddr.sin_port = htons(8084);

    clientlen = sizeof(servaddr);
    connect(clientfd,(struct sockaddr *)&servaddr,clientlen);

    MessagePassing message(clientfd);

    //Send message to server
    message.sendMessage("Hello from client");

    //Receive from server
    std::string msg = message.receiveMessage();
    std::cout<< msg << std::endl;
    return 1;
}
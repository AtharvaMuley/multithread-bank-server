//Messagepassing.h
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string>

#ifndef MessagePassing_H
#define MessagePassing_H

class MessagePassing{

    //Class pubic variables
    public:
    int sockfd;
    char buffer[1024]={0};
    const int MSG_OK = 0;
    const int MSG_ERR = -1;

    //Constructor
    MessagePassing(int);
    
    //Class methods
    void sendMessage(char msg[]);
    char* receiveMessage();
};

#endif
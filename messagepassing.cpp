#include "messagepassing.h"

MessagePassing::MessagePassing(int socketfd){
        sockfd = socketfd;
}
    
void MessagePassing::sendMessage(char msg[]){
    send(sockfd,msg,strlen(msg),0);
}

char* MessagePassing::receiveMessage(){
    int valread = read(sockfd,buffer,1024);
    //send(sockfd,(char *) MSG_OK,2,0);
    //Receive MSG state from other side
    // int msg_type = read(sockfd,buffer,1024);
    // if (buffer[1] == 'd'){
    //     //Deposit
    //     retrurn 
    // }
    // else if(buffer[1] == 'w'){
    //     //widthdraw
    // }
    // else if(buffer[1] == 'b'){
    //     //balance
    // }
    // else if(buffer[1] == 'm'){
    //     send(sockfd,(char *)'m',1,0);
    //     return buffer;
    // }
    // else{
    //     std::cout << "Error Seding Message" << std::endl;
    // }
    return buffer;
}



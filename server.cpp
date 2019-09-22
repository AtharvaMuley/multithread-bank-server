#include <iostream>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <string>
#include <pthread.h>
#include "messagepassing.h"
#include "bank_account.h"

#define NO_OF_THREADS 50

int thread_counter;

std::string* parseClientData(std::string clientMsg){
    std::string* climsg = new std::string[4];
    std::smatch match;
    std::regex re_transaction_parse("(.[0-9]*)\\s(.[0-9]*)\\s(.[a-zA-Z]*)\\s(.[0-9]*)");
    std::regex_search(clientMsg, match, re_transaction_parse);
    if(match.size()>1){
        climsg[0] = match.str(0);
        climsg[1] = match.str(1);
        climsg[2] = match.str(2);
        climsg[3] = match.str(3);
    }
    return climsg;
}

void *worker(void *arg){
    int clientfd = (long)arg;
    std::cout<<"Connection successful: "<<clientfd<< std::endl;
    MessagePassing message(clientfd);

    //Receive from client
    std::string msg = message.receiveMessage();
    //std::cout<< msg << std::endl;

    //Print Parse data
    std::string* parsedData = parseClientData(msg);
    std::cout << parsedData[0] << std::endl;
    std::cout << parsedData[1] << std::endl;
    std::cout << parsedData[2] << std::endl;
    std::cout << parsedData[3] << std::endl;

    //Send message to client
    message.sendMessage("Hello from server");

    
    sleep(3);
    std::cout<<"Exiting client\ns";
    close(clientfd);
    thread_counter--;
    pthread_exit(NULL);
}

using namespace std;
int main(int argc, char *arhv[]){
    int sockfd,clientfd;
    struct sockaddr_in servaddr,cliaddr;
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd < 0 ){
        std::cerr << "Error: " << std::endl;
    }

    //fill servaddr with zeros 
    // memset(&servaddr,0, sizeof(struct sockaddr_in));

    //Assign IP with port 
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(8084);

    //bind ip with port 
    bind(sockfd,(struct sockaddr *) &servaddr, sizeof(servaddr));

    listen(sockfd,5);
    //std::cout << "Server IP: " << servaddr.sin_addr.s_addr << " Port: " << servaddr.sin_port << endl;
    std::cout << "SERVER is now in Listening mode...\n";

    int addrlen,rc;
    addrlen = sizeof(cliaddr);
    pthread_t threads[5];
    thread_counter = 0;
    while (1)
    {
    clientfd = accept(sockfd,(struct sockaddr * )&cliaddr,(socklen_t *)&addrlen);
    if(clientfd < 0 ){
        cout<< "Error creating connetion"<<endl;
        return -1;
    }
    else{
        rc = pthread_create(&threads[thread_counter],NULL,worker,(void *)clientfd);
        // pthread_join(threads[thread_counter],NULL);
        thread_counter++;
        while (thread_counter > NO_OF_THREADS)
        {
            
        }
        std::cout<<"Thread:"<<thread_counter << endl;
    }
    }
    return 1;
}
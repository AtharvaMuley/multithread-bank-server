#include <iostream>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <string>
#include <pthread.h>
#include "messagepassing.h"


#define NO_OF_THREADS 50

int thread_counter;

void *worker(void *arg){
    int clientfd = (long)arg;
    std::cout<<"Connection successful: "<<clientfd<< std::endl;
    MessagePassing message(clientfd);

    //Receive from client
    std::string msg = message.receiveMessage();
    std::cout<< msg << std::endl;

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
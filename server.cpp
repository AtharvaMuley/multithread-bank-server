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
BankAccounts account;

std::string* parseClientData(std::string clientMsg){
    std::string* climsg = new std::string[4];
    std::smatch match;
    std::regex re_transaction_parse("([0-9]*)\\s(.[0-9]*)\\s(.[a-zA-Z]*)\\s(.[0-9]*)");
    std::regex_search(clientMsg, match, re_transaction_parse);
    if(match.size()>4){
        climsg[0] = match.str(1);
        climsg[1] = match.str(2);
        climsg[2] = match.str(3);
        climsg[3] = match.str(4);
    }
    return climsg;
}

void *interestDeamon(void *arg){
    while(1){
        account.intrest();
    sleep(5);
    }
}

void *worker(void *arg){
    int clientfd = (long)arg;
    
    pthread_mutex_t lock;
    // std::cout<<"Connection successful: "<<clientfd<< std::endl;
    MessagePassing message(clientfd);

    //Receive from client
    std::string msg = message.receiveMessage();
    //std::cout<< msg << std::endl;

    //Print Parse data
    std::string* parsedData = parseClientData(msg);

    // std::cout << account.fetchBalance(stoi(parsedData[1]))<<std::endl;
    // std::cout << parsedData[0] << std::endl;
    // std::cout << parsedData[1] << std::endl;
    // std::cout << parsedData[2] << std::endl;
    // std::cout << parsedData[3] << std::endl;

    //Try to accquire lock
    // if (account.islockable(stoi(parsedData[1]))){ 
    //     pthread_mutex_lock(&lock);

    // }

    std::cout << "Transacting for accountID: "<<parsedData[1]<< " from ClientID: "<<clientfd<<std::endl;
    account.islockable(stoi(parsedData[1]));
    //withdraw money
    sleep(3);

    if(parsedData[2].compare("w") || parsedData[2].compare("W")){
        // std::cout << "Withdraw" << std::endl;
        int transactionStatus = account.withdraw(stoi(parsedData[1]),stoi(parsedData[3]));
        if (transactionStatus == 1){
            message.sendMessage("Withdraw Transaction Successful!");
        }
        else if(transactionStatus == -1){
            message.sendMessage("Insufficient Funds!");
        }
        else
        {
            message.sendMessage("Withdraw Transaction Unsuccessful!");
        }
        
        
    }
    //Deposit money
    else if(parsedData[2].compare("d") || parsedData[2].compare("D")){
        // std::cout << "Deposit" << std::endl;
        int transactionStatus = account.deposit(stoi(parsedData[1]),stoi(parsedData[3]));
        if (transactionStatus == 1){
            message.sendMessage("Deposit Transaction Successful!");
        }
        else
        {
            message.sendMessage("Deposit Transaction Unsuccessful!");
        }
        
        
    }
    //Send message to client
    //message.sendMessage("Hello from server");

    account.removeLock(stoi(parsedData[1]));
    
    // std::cout<<"Exiting client\ns";
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
    servaddr.sin_port = htons(8085);

    //bind ip with port 
    bind(sockfd,(struct sockaddr *) &servaddr, sizeof(servaddr));

    listen(sockfd,5);
    //std::cout << "Server IP: " << servaddr.sin_addr.s_addr << " Port: " << servaddr.sin_port << endl;
    std::cout << "SERVER is now in Listening mode...\n";

    int addrlen,rc;
    addrlen = sizeof(cliaddr);
    account.init();
    pthread_t threads[5];
    thread_counter = 0;
    pthread_t interestThread;
    rc = pthread_create(&interestThread,NULL, interestDeamon,NULL);
    pthread_join(interestThread,NULL);
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
        // std::cout<<"Thread:"<<thread_counter << endl;
    }
    }
    return 1;
}
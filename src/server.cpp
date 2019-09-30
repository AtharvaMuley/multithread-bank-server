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
	std::cout<< "Started calculating Interest\n";
    while(1){
        account.interest();
    sleep(15);
    }
}

void *worker(void *arg){
    int clientfd = (long)arg;
    
    pthread_mutex_t lock;
    // std::cout<<"Connection successful: "<<clientfd<< std::endl;
    MessagePassing message(clientfd);

    //Receive from client
    std::string msg = message.receiveMessage();
    std::cout<< msg << std::endl;

    //Print Parse data
    std::string* parsedData = parseClientData(msg);

    // std::cout << account.fetchBalance(stoi(parsedData[1]))<<std::endl;
    // std::cout << parsedData[0] << std::endl;
    // std::cout << parsedData[1] << std::endl;
    // std::cout << parsedData[2] << std::endl;
    // std::cout << parsedData[3] << std::endl;

    std::cout << "Transacting for accountID: "<<parsedData[1]<< " from ClientID: "<<clientfd<<std::endl;
    account.islockable(stoi(parsedData[1]));
    //withdraw money
    //sleep(3);

    if(parsedData[2] == "w" || parsedData[2]=="W"){
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
    else if(parsedData[2] == "d" || parsedData[2] == "D"){
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
    
    std::cout<<"Exiting client\ns";
    close(clientfd);
    thread_counter--;
    pthread_exit(NULL);
}

int main(int argc, char *arhv[]){
    int sockfd,clientfd;
    struct sockaddr_in servaddr,cliaddr;
    sockfd = socket(PF_INET, SOCK_STREAM, 0);
    int optval = 1;
    setsockopt(sockfd,SOL_SOCKET, SO_REUSEPORT, &optval, sizeof(optval));
    if(sockfd < 0 ){
        std::cerr << "Socket Error: " << std::endl;
	exit(0);
    }

    //fill servaddr with zeros 
    // memset(&servaddr,0, sizeof(struct sockaddr_in));

    //Assign IP with port 
    servaddr.sin_family = PF_INET;
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(8090);

    //bind ip with port 
    if(bind(sockfd,(struct sockaddr *) &servaddr, sizeof(servaddr)) < 0 ){
        std::cerr << "Binding Error: " << std::endl;
	exit(0);
    }

    if(listen(sockfd,5) < 0){
        std::cerr << "Listening Error: " << std::endl;
	exit(0);
    }
    //std::cout << "Server IP: " << servaddr.sin_addr.s_addr << " Port: " << servaddr.sin_port << endl;
    std::cout << "SERVER is now in Listening mode...\n";

    int addrlen,rc;
    addrlen = sizeof(cliaddr);
    account.init();
    pthread_t threads[5];
    thread_counter = 0;
    pthread_t interestThread;
    //rc = pthread_create(&interestThread,NULL, interestDeamon,NULL);
    
    while (1)
    {
    clientfd = accept(sockfd,(struct sockaddr * )&cliaddr,(socklen_t *)&addrlen);
    if(clientfd < 0 ){
        std::cout<< "Error creating connetion"<< std::endl;
        continue;
    }
    else{
	//std::cout<<"Thread:"<<thread_counter << std::endl;
        rc = pthread_create(&threads[thread_counter],NULL,worker,(void *)clientfd);
        pthread_join(threads[thread_counter],NULL);
        thread_counter++;
        while (thread_counter > NO_OF_THREADS){}
        
    }
	
    }
    pthread_join(interestThread,NULL);
    return 1;
}

#include <iostream>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <string>
#include <ctime>
#include <pthread.h>
#include "messagepassing.h"
#include "bank_account.h"

#define NO_OF_THREADS 50

int thread_counter;
BankAccounts account;

/*
	parseClientData is used to Parse the data sent by the client
*/
std::string* parseClientData(std::string clientMsg){
    std::string* climsg = new std::string[4];
    std::smatch match;
    // Parse the message using message
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

/*
	interestDeamon is used to give interest to clients every 15 seconds.
*/
void *interestDeamon(void *arg){
    while(1){
    	time_t timeNow = time(0);
		tm *time = localtime(&timeNow);
		std::cout<< "Interest Paid on: "<<time->tm_hour<<":"<<time->tm_min<<":"<<time->tm_sec<<std::endl;
        account.interest();
    	sleep(15);
    }
}

/*
	worker in the main function which manages the client's connections.
*/
void *worker(void *arg){
    int clientfd = *((int *)arg);
    pthread_mutex_t lock;
    char response[512] = {0};
    MessagePassing message(clientfd);

    //Receive from client
    std::string msg = message.receiveMessage();

    //Parse the data
    std::string* parsedData = parseClientData(msg);

    std::cout << "Transacting for accountID: "<<parsedData[1]<< " from ClientID: "<<clientfd<<std::endl;
    
    // Check if the mutex associated with that specific account number is available. 
    account.islockable(stoi(parsedData[1]));
    
	/*
	* Check if the client wishes to perform widthdraw function
	*/
    if(parsedData[2] == "w" || parsedData[2]=="W"){
        // std::cout << "Withdraw" << std::endl;
        int transactionStatus = account.withdraw(stoi(parsedData[1]),stoi(parsedData[3]));
        if (transactionStatus == 1){
        	strcpy(response,"Withdraw Transaction Successful!");
            message.sendMessage(response);
        }
        else if(transactionStatus == -1){
        	strcpy(response,"Insufficient Funds!");
            message.sendMessage(response);
        }
        else
        {
        	strcpy(response,"Withdraw Transaction Unsuccessful!");
            message.sendMessage(response);
        }
    }
    
    /*
	* Check if the client wishes to perform deposit function
	*/
    else if(parsedData[2] == "d" || parsedData[2] == "D"){
        int transactionStatus = account.deposit(stoi(parsedData[1]),stoi(parsedData[3]));
        if (transactionStatus == 1){
        	strcpy(response,"Deposit Transaction Successful!");
            message.sendMessage(response);
        }
        else
        {
        	strcpy(response,"Deposit Transaction Unsuccessful!");
            message.sendMessage(response);
        }
        
        
    }

	// Release the mutex associated with that specific account number.
    account.removeLock(stoi(parsedData[1]));
    
    //Close the client connection
    close(clientfd);
    thread_counter--;
    pthread_exit(NULL);
}

int main(int argc, char *arhv[]){
	int addrlen,rc;
    int sockfd,clientfd;
    pthread_t threads[5],interestThread;
    thread_counter = 0;
    
    struct sockaddr_in servaddr,cliaddr;
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    int optval = 1;
    setsockopt(sockfd,SOL_SOCKET, SO_REUSEPORT, &optval, sizeof(optval));
    if(sockfd < 0 ){
        std::cerr << "Socket Error: " << std::endl;
	exit(0);
    }

    //fill servaddr with zeros 
    memset(&servaddr,0, sizeof(struct sockaddr_in));

    //Assign IP with port 
    servaddr.sin_family = AF_INET;
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
    //std::cout << "Server IP: " << inet_ntoa(servaddr.sin_addr) << " Port: " << servaddr.sin_port<< std::endl;
    std::cout << "SERVER is now in Listening mode...\n";

    
    addrlen = sizeof(cliaddr);
    account.init();
    
    // Start the intrest Thread
    rc = pthread_create(&interestThread,NULL, interestDeamon,NULL);
    
    while (1)
    {
    	clientfd = accept(sockfd,(struct sockaddr * )&cliaddr,(socklen_t *)&addrlen);
    	if(clientfd < 0 ){
        	std::cout<< "Error creating connetion"<< std::endl;
        	continue;
    	}
    	else{
        	rc = pthread_create(&threads[thread_counter],NULL,worker,(void *)&clientfd);
        	thread_counter++;
        	while (thread_counter > NO_OF_THREADS){
        		/* While thread limit had reached wait till the current clients close the connection*/
        	}  
    }
    }
    pthread_join(interestThread,NULL);
    return 1;
}

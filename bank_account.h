#include <iostream>
#include <list>
#include <iterator>
#include <string>
#include <fstream>
#include <regex>
#include<stdlib.h>

#ifndef Bank_Account_H
#define Bank_Account_H


class BankAccounts{
    private:
    struct bank_account
    {
        int accountId;
        std::string name;
        long balance;
        bool is_locked = false;
    };
    int no_of_accounts;
    std::list<struct bank_account> customer_accounts;

    public:
    //constructor
    BankAccounts(){
        init();
    }
    void init(){}
    void addAccount(int accountNo, std::string name, long balance){}
    void viewAllAccountInfo(){}
    bool islockable(int){}
    long fetchBalance(int){}
    void updateBalance(int,long){}
    struct bank_account fetchAccountInfo(int accountId){}
};

#endif
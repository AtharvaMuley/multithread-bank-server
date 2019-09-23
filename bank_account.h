#ifndef Bank_Account_H
#define Bank_Account_H

#include <iostream>
#include <list>
#include <iterator>
#include <string>
#include <fstream>
#include <regex>
#include<stdlib.h>

class BankAccounts{
    private:
    struct bank_account
    {
        int accountId;
        std::string name;
        int balance;
        bool is_locked = false;
    };
    int no_of_accounts;
    std::list<struct bank_account> customer_accounts;

    public:
    //constructor
    BankAccounts();
    void init();
    void addAccount(int accountNo, std::string name, long balance);
    void viewAllAccountInfo();
    bool islockable(int);
    int fetchBalance(int);
    int withdraw(int accountNo, int amount);
    void updateBalance(int,int);
    // struct bank_account fetchAccountInfo(int accountId);
};

#endif
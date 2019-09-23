#include <iostream>
#include "transaction.h"
#include "bank_account.h"

int withdraw(int accountNo, int amount){
    BankAccounts account;
    long balance = account.fetchBalance(accountNo);
    std::cout << balance << std::endl;
    if (amount > balance){
        return -1;
    }
    else{
        balance -= amount;
        account.updateBalance(accountNo,balance);
        return 1;
    }
}
#include <iostream>
#include "transaction.h"
#include "bank_account.h"

int withdraw(int accountNo, float amount){
    BankAccounts account;
    long balance = account.fetchBalance(accountNo);
    if (amount > balance){
        return -1;
    }
    else{
        balance -= amount;
        account.updateBalance(accountNo,balance);
        return 1;
    }
}
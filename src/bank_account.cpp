#include "bank_account.h"

BankAccounts::BankAccounts(){
}
void BankAccounts::init(){
    //Read Records.txt
    std::string line;
    std::ifstream fp("Records.txt");
    if (fp.is_open()){
        std::smatch match,match1,match2;
        std::regex re_acntno("(.[0-9]*)");
        std::regex re_name("(\\s.[a-zA-Z]*.[0-9]*\\s)");
        std::regex re_bal("\\s.[a-zA-Z]*.[0-9]*\\s(.[0-9]*)");
        while(getline(fp,line)){
            struct bank_account tmp;

            //Regex for searching Account No
            std::regex_search(line, match, re_acntno);
            if(match.size()>1){
                tmp.accountId = std::stoi(match.str(1));
            }

            //Regex for searching Name
            std::regex_search(line, match1, re_name);
            if(match1.size()>1){
                tmp.name = match1.str(1);
            }
            //Regex for searching balance
            std::regex_search(line, match2, re_bal);
            if(match2.size()>1){
                tmp.balance = (long) std::stoi(match2.str(1));
            }
            addAccount(std::stoi(match.str(1)),match1.str(1), (long) std::stoi(match2.str(1)));
            no_of_accounts++;
        }
        fp.close();
    }
    else{
        std::cout << "Unable to open Records.txt" << std::endl;
        std::_Exit(EXIT_FAILURE);
    }

}
void BankAccounts::addAccount(int accountNo, std::string name, long balance){
    struct bank_account tmp;
    tmp.accountId = accountNo;
    tmp.name  = name;
    tmp.balance = balance;
    customer_accounts.push_back(tmp);
}
void BankAccounts::viewAllAccountInfo(){
    std::list<struct bank_account>:: iterator it;
    for(it =customer_accounts.begin();it != customer_accounts.end();++it){
        std::cout<< "Account Id: "<< it->accountId <<" Name: "<< it->name << " Balance: "<< it->balance << std::endl;
    }
}
void BankAccounts::interest(){
    std::list<struct bank_account>:: iterator it;
    for(it =customer_accounts.begin();it != customer_accounts.end();++it){
        islockable(it->accountId);
        int balance = fetchBalance(it->accountId);
        balance += balance*0.005;
        updateBalance(it->accountId,balance);
        removeLock(it->accountId);
    }
}
bool BankAccounts::islockable(int accountId){
    struct bank_account temp;
    std::list<struct bank_account>:: iterator it;
    for(it =customer_accounts.begin();it != customer_accounts.end();++it){
        if(it->accountId == accountId){
            pthread_mutex_lock(&it->mutex);
            pthread_cond_wait(&it->lock,&it->mutex); 
            return true; 
        }
    }
    return false; 
}
bool BankAccounts::removeLock(int accountId){
    struct bank_account temp;
    std::list<struct bank_account>:: iterator it;
    for(it =customer_accounts.begin();it != customer_accounts.end();++it){
        if(it->accountId == accountId){
            pthread_cond_signal(&it->lock);
            pthread_mutex_unlock(&it->mutex);
            return true; 
        }
    }
    return false;
}
int BankAccounts::fetchBalance(int accountId){
    struct bank_account temp;
    std::list<struct bank_account>:: iterator it;
    for(it =customer_accounts.begin();it != customer_accounts.end();++it){
        if(it->accountId == accountId){
            return it->balance; 
        }
    }
    return -1;
}
int BankAccounts::withdraw(int accountNo, int amount){
    int balance = fetchBalance(accountNo);
    if (amount > balance){
        return -1;
    }
    else{
        balance -= amount;
        updateBalance(accountNo,balance);
        return 1;
    }
}
int BankAccounts::deposit(int accountNo, int amount){
    int balance = fetchBalance(accountNo);
    std::cout << balance << std::endl;
        balance += amount;
        updateBalance(accountNo,balance);
        return 1;
}
void BankAccounts::updateBalance(int accountId,int updatedBalance){
    std::list<struct bank_account>:: iterator it;
        for(it =customer_accounts.begin();it != customer_accounts.end();it++){
            if(it->accountId == accountId){
                std::cout << "Updated balance for AccountId: "<< accountId <<" is: "<<updatedBalance << std::endl;
                it->balance = updatedBalance;
                return;
            }
        }
}
// struct bank_account BankAccounts::fetchAccountInfo(int accountId){
//     struct bank_account temp;
//     std::list<struct bank_account>:: iterator it;
//     for(it =customer_accounts.begin();it != customer_accounts.end();++it){
//         if(it->accountId == accountId){
//             return it; 
//         }
//     }
//     temp.accountId = accountId;
//     temp.balance = -1;
//     temp.name = "Not Found";
//     return temp;    
// }

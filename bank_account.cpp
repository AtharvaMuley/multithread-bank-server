#include <iostream>
#include <list>
#include <iterator>
#include <string>
#include <fstream>
#include <regex>
#include<stdlib.h>

#define MAX_NO_OF_ACCOUNTS 10

class BankAccounts{
    private:
    struct bank_account
    {
        int accountId;
        std::string name;
        long balance;
    };
    int no_of_accounts;
    std::list<struct bank_account> customer_accounts;

    public:
    //constructor
    BankAccounts(){
        init();
    }
    void init(){
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
            }
            fp.close();
        }
        else{
            std::cout << "Unable to open Records.txt" << std::endl;
            std::_Exit(EXIT_FAILURE);
        }

    }
    void addAccount(int accountNo, std::string name, long balance){
        struct bank_account tmp;
        tmp.accountId = accountNo;
        tmp.name  = name;
        tmp.balance = balance;
        customer_accounts.push_back(tmp);
    }
    void viewAccountInfo(){
        std::list<struct bank_account>:: iterator it;
        for(it =customer_accounts.begin();it != customer_accounts.end();++it){
            std::cout<< "Account Id: "<< it->accountId <<" Name: "<< it->name << " Balance: "<< it->balance << std::endl;
        }
    }
};

int main(){
    BankAccounts accounts;
    accounts.viewAccountInfo();
}
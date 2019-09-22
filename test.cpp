#include <iostream>
#include <list>
using namespace std;
string* ret(){
    string* c = new string[4];
    c[0] = "Hello";
    c[1] = "Hello3";
    c[2] = "Hello4";
    c[3] = "Hello5";
    return c;
}

void iterator(std::list<struct bank_account> q){

}
int main(){
    string *tmp = ret();
    cout << tmp[0] << endl;
    cout << tmp[1] << endl;
    cout << tmp[2] << endl;
    cout << tmp[3] << endl;

}

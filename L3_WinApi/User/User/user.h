#ifndef USER
#define USER


#include <iostream>
#include <limits>
using namespace std;
#include <string>
#include <sstream>

class cHuman
{
private:

    int id;
    string name;
    int accountMoney;
    int deleteMoney;

public:
    cHuman(int id = 12018,string name = "UnknownEnot",int money = 500);
    string transaction_prepare();
    void transaction_confirm(bool);
    int moneyAmount();
    string getName();
    void setName(string name);
};

#endif // USER


#ifndef CASHMASHINE_H
#define CASHMASHINE_H

#include <iostream>
using namespace std;
#include <string>
#include <fstream>
#include <sstream>
#include <time.h>

class CashMachine
{
private:
    int id;
    int USD_amount;
    int EUR_amount;
    int BYN_amount;

public:
    CashMachine(int id = 156335);
    string transaction_request(string);
    void save_state();
    bool restore_state();
    void set_money_amount(int BYN, int USD, int EUR);
    string status();
    int getId();
};

#endif // CASHMASHINE_H

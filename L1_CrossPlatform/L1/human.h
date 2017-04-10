#ifndef HUMAN_H
#define HUMAN_H

#include <iostream>
#include <limits>
using namespace std;
#include <string>
#include <sstream>

class Human
{
private:
    int id;
    string name;
    int money_amount;
    int frozen_amount;

public:
    Human(int id = 1000, string name = "admin", int money_amount = 200);
    string transaction_prepare();
    void transaction_confirm(bool);
    int moneyAmount();
    string getName();
};

#endif // HUMAN_H


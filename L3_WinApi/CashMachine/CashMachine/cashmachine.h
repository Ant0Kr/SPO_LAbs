#ifndef CASHMACHINE
#define CASHMACHINE

#endif // CASHMACHINE

#include <iostream>
using namespace std;
#include <string>
#include <fstream>
#include <sstream>
#include <time.h>

class cMachine
{
private:
    int USD;
    int BYR;
    int EUR;

public:
    cMachine();
    string transaction_request(string);
    void save_state();
    bool restore_state();
    void set_money_amount(int BYN, int USD, int EUR);
    string status();
    int getId();
};

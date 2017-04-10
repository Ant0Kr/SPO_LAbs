#include "cashmachine.h"

CashMachine::CashMachine(int id)
{
    this->id = id;
    if(!restore_state())
        set_money_amount(250,30,100);
}

void CashMachine::set_money_amount(int BYN, int USD, int EUR)
{
    this->BYN_amount = BYN;
    this->USD_amount = USD;
    this->EUR_amount = EUR;
}

string CashMachine::transaction_request(string request)
{
    time_t seconds = time(NULL);
    string time(asctime(localtime(&seconds)));
    time.erase(time.size()-1);

    stringstream buf(request);
    buf.ignore(6);

    string usr_name;
    buf >> usr_name;

    buf.ignore(10);
    string currency;
    buf >> currency;

    int money_request;
    buf >> money_request;

    cout << '[' << time << "] - " << "User: " << usr_name << " Currency: " << currency << ' ' << money_request << " - ";

    string transaction_state;

    if(currency == "BYN")
    {
        if(money_request <= BYN_amount)
        {
            BYN_amount -= money_request;
            transaction_state = "Transaction is confirmed!";
        }
        else transaction_state = "Transaction is declined!";
    }
    else
        if(currency == "USD")
        {
            if(money_request <= USD_amount)
            {
                USD_amount -= money_request;
                transaction_state = "Transaction is confirmed!";
            }
            else transaction_state = "Transaction is declined!";
        }
        else
            if(currency == "EUR")
            {
                if(money_request <= EUR_amount)
                {
                    EUR_amount -= money_request;
                    transaction_state = "Transaction is confirmed!";
                }
                else transaction_state = "Transaction is declined!";
            }
    cout << transaction_state << endl;

    return transaction_state;
}

void CashMachine::save_state()
{
    fstream file;
    file.open("CashMashine.txt",ios::in | ios::out | ios::trunc);
    file << "BYN: " << BYN_amount << endl;
    file << "USD: " << USD_amount << endl;
    file << "EUR: " << EUR_amount << endl;
    return;
}

bool CashMachine::restore_state()
{
    fstream file;
    file.open("CashMashine.txt",ios::in);
    if(!file.is_open()) return false;
    file.ignore(5);
    file >> BYN_amount;
    file.ignore(5);
    file >> USD_amount;
    file.ignore(5);
    file >> EUR_amount;
    return true;
}

string CashMachine::status()
{
    stringstream buf;
    buf << "BYN: " << BYN_amount << " USD: " << USD_amount << " EUR: " << EUR_amount;
    return buf.str();
}


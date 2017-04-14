#include "cashmachine.h"

cMachine::cMachine()
{
    if(!restore_state())
    {
        BYR = 1000;
        USD = 150;
        EUR = 100;
    }
}

bool cMachine::restore_state()
{
    fstream file;
    file.open("CashMashine.txt",ios::in|ios::out);
    if(!file.is_open())
        return false;
    file.ignore(5);
    file >> BYR;

    file.ignore(5);
    file >> USD;
    file.ignore(5);
    file >> EUR;
    return true;
}

string cMachine::transaction_request(string request)
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

    cout << '[' << time << "]" <<endl<<
            "User: " << usr_name <<endl<<
            "Currency: " << money_request << ' ' << currency  ;

    string transaction_state;

    if(currency == "BYR")
    {
        if(money_request <= BYR)
        {
            BYR -= money_request;
            transaction_state = "Transaction is confirmed!";
        }
        else transaction_state = "Transaction is declined!";
    }
    else
        if(currency == "USD")
        {
            if(money_request <= USD)
            {
                USD -= money_request;
                transaction_state = "Transaction is confirmed!";
            }
            else transaction_state = "Transaction is declined!";
        }
        else
            if(currency == "EUR")
            {
                if(money_request <= EUR)
                {
                    EUR -= money_request;
                    transaction_state = "Transaction is confirmed!";
                }
                else transaction_state = "Transaction is declined!";
            }
    cout << endl << transaction_state << endl;
    cout << "**************************"<<endl;

    return transaction_state;
}

void cMachine::save_state()
{
    fstream file;
    file.open("CashMashine.txt",ios::in | ios::out | ios::trunc);
    file << "BYR: " << BYR << endl;
    file << "USD: " << USD << endl;
    file << "EUR: " << EUR << endl;
    return;
}

string cMachine::status()
{
    stringstream buf;
    buf << "BYR: " << BYR << " USD: " << USD << " EUR: " << EUR;
    return buf.str();
}



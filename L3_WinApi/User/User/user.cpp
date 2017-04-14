#include "user.h"

cHuman::cHuman(int id,string name,int money)
{
    this->id = id;
    this->name = name;
    this->accountMoney = money;
}

string cHuman::transaction_prepare()
{
    cout << "Select a currency:\n";
    cout << "0.Cancel\n";
    cout << "1.BYR\n";
    cout << "2.USD\n";
    cout << "3.EUR\n";

    int menu;
    while(!(cin >> menu) || menu < 0 || menu > 3)
    {
        cout << "Please input correct value!\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    stringstream buf;

    buf << "User: " << name << ' ';
    buf << "Currency: ";

    int conversion;
    switch(menu)
    {
    case 0: return "";
    case 1: buf << "BYR "; conversion = 1; break;
    case 2: buf << "USD "; conversion = 2; break;
    case 3: buf << "EUR "; conversion = 3; break;
    }

    cout << "Please, enter enter the amount: ";

    int amount;
    while(!(cin >> amount) || amount < 0 || amount * conversion > accountMoney)
    {
        cout << "Please input correct value!\n";
        cout << "Money in the account: " << accountMoney << " BYN" << endl;
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    deleteMoney = amount * conversion;

    buf << amount;

    return buf.str();

}

void cHuman::transaction_confirm(bool is_aprooved)
{
    if(is_aprooved)
    {
        accountMoney -= deleteMoney;
        cout << "Money in the account: " << accountMoney << " BYN" << endl;
    }
    else
        cout << "Money in the account: " << accountMoney << " BYN" << endl;

    deleteMoney = 0;
}

int cHuman::moneyAmount()
{
    return accountMoney;
}

string cHuman::getName()
{
    return name;
}

void cHuman::setName(string name)
{
    this->name = name;
}

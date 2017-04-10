#include "human.h"

Human::Human(int id, string name, int money_amount)
{
    this->id = id;
    this->name = name;
    this->money_amount = money_amount;
    this->frozen_amount = 0;
}

string Human::transaction_prepare()
{
    cout << "Select a currency:\n";
    cout << "0.Cancel\n";
    cout << "1.BYN\n";
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
    case 1: buf << "BYN "; conversion = 1; break;
    case 2: buf << "USD "; conversion = 2; break;
    case 3: buf << "EUR "; conversion = 3; break;
    }

    cout << "Please, enter enter the amount: ";

    int amount;
    while(!(cin >> amount) || amount < 0 || amount * conversion > money_amount)
    {
        cout << "Please input correct value!\n";
        cout << "Money in the account: " << money_amount << " BYN" << endl;
        cin.clear();
	cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    frozen_amount = amount * conversion;

    buf << amount;

    return buf.str();
}

void Human::transaction_confirm(bool is_aprooved)
{
    if(is_aprooved)
    {
        money_amount -= frozen_amount;
        frozen_amount = 0;
        cout << "Money in the account: " << money_amount << " BYN" << endl;
    }
    else
    {
        frozen_amount = 0;
        cout << "Money in the account: " << money_amount << " BYN" << endl;
    }
}

int Human::moneyAmount()
{
    return money_amount;
}

string Human::getName()
{
    return name;
}

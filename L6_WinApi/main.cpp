#include <iostream>
using namespace std;

#include "project6.h"

int main(int argc, char *argv[])
{
    char * test;

    cout << "Hello!" << endl;
    cout << "/**********************************************************************/" << endl;

    cout << "Now we try to allocate 100 bytes to char array:" << endl;

    test = (char *)Project6::malloc(100);

    if(test)
    {
        cout << "Address of allocated memory in hex: " << hex << int(test) << endl;
        cout << "/**********************************************************************/" << endl;
        cout << "Now we try to copy some information in our array" << endl;

        strcpy(test,"Some information(here can by everything)");

        cout << "Information in our array [] and address []: ";
        cout << "[" << test << "]-";
        cout << "[" << hex << int(test) << "]" << endl;
        cout << "/**********************************************************************/" << endl;

        cout << "Now we try to reallocate to 1024 bytes:" << endl;

        test = (char *)Project6::realloc(test, 1024);

        if(test)
        {
            cout << "Information in our array [] and address []: ";
            cout << "[" << test << "]-";
            cout << "[" << hex << int(test) << "]" << endl;
            cout << "/**********************************************************************/" << endl;

            cout << "Now we try to free memory:" << endl;

            Project6::free(test);
        }
        else
        {
            cout << "I'am sorry but we can't anything next to do.";
        }

    }
    else
    {
        cout << "I'am sorry but we can't anything next to do.";
    }

    cout << "/**********************************************************************/" << endl;
    cout << "Goodbye!" << endl;

    return 0;
}


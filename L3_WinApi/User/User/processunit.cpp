#include "processunit.h"

HANDLE cProcessUnit::hNamedWritePipe;
LPSTR cProcessUnit::hPipeWriteName = "\\\\.\\pipe\\$SecurityATmWriteLine$";
HANDLE cProcessUnit::hNamedPerformPipe;
LPSTR cProcessUnit:: hPipePerformName = "\\\\.\\pipe\\$SecurityATmPerformLine$";

HANDLE cProcessUnit::hPerformEvent;
HANDLE cProcessUnit::hWriteEvent;
HANDLE cProcessUnit::hUserPerformEvent;
HANDLE cProcessUnit::hUserWriteEvent;
HANDLE cProcessUnit::hWriteConnectEvent;
HANDLE cProcessUnit::hPerformConnectEvent;

HANDLE cProcessUnit::hWriteSemaphore;
HANDLE cProcessUnit::hPerformSemaphore;

string cProcessUnit::name;
void cProcessUnit::StartProgram()
{
    SetConsoleCtrlHandler((PHANDLER_ROUTINE)cProcessUnit::Exit,true);

    cHuman human;

    hPerformSemaphore = OpenSemaphore(SEMAPHORE_ALL_ACCESS, false, "PerformSemaphore");
    hWriteSemaphore = OpenSemaphore(SEMAPHORE_ALL_ACCESS, false, "WriteSemaphore");

    hUserWriteEvent = OpenEvent(EVENT_ALL_ACCESS,FALSE,"hUserWriteEvent");
    hWriteEvent = OpenEvent(EVENT_ALL_ACCESS,FALSE,"hWriteEvent");

    hWriteConnectEvent = OpenEvent(EVENT_ALL_ACCESS,FALSE,"hWriteConnectEvent");
    hPerformConnectEvent = OpenEvent(EVENT_ALL_ACCESS,FALSE,"hPerformConnectEvent");

    hUserPerformEvent = OpenEvent(EVENT_ALL_ACCESS,FALSE,"hUserPerformEvent");
    hPerformEvent = OpenEvent(EVENT_ALL_ACCESS,FALSE,"hPerformEvent");

    cout << "Please input name:" << endl;
    cin >> name;
    human.setName(name);
    cout << "Please waiting your quene!" << endl;
    string comand("title User: " + human.getName());
    system(comand.c_str());

    WaitForSingleObject(hWriteSemaphore,INFINITE);

    SetEvent(hWriteConnectEvent);
    WaitForSingleObject(hUserWriteEvent,INFINITE);
    Sleep(50);

    hNamedWritePipe = CreateFile(hPipeWriteName, GENERIC_READ | GENERIC_WRITE,
                            0, NULL, OPEN_EXISTING, 0, NULL);
    string request = human.getName();
    int i = request.size();
    WriteFile(hNamedWritePipe, reinterpret_cast<char*>(&i), sizeof(int), NULL, NULL);
    WriteFile(hNamedWritePipe, request.c_str(), i, NULL, NULL);

    SetEvent(hWriteEvent);

    WaitForSingleObject(hPerformSemaphore, INFINITE);

    SetEvent(hPerformConnectEvent);
    WaitForSingleObject(hUserPerformEvent,INFINITE);
    Sleep(50);
    hNamedPerformPipe = CreateFile(hPipePerformName, GENERIC_READ | GENERIC_WRITE,
                            0, NULL, OPEN_EXISTING, 0, NULL);

    while(true)
    {
        system("cls||clear");
        cout << "Account menu:" << endl;
        cout << "1.State of account" << endl;
        cout << "2.Withdraw money" << endl;
        cout << "3.Admin: ATM status" << endl;
        cout << "4.Admin: shutdown system" << endl;

        int menu;

        while(!(cin >> menu) || menu < 1 || menu > 4)
        {
            cout << "Please input correct value!\n";
            cin.clear();
            cin.ignore(cin.rdbuf()->in_avail());
        }

        switch (menu)
        {
        case 1:
        {
            system("cls||clear");
            cout << "You have: " << human.moneyAmount() << " BYR" << endl;
            system("pause");
            break;
        }
        case 2:
        {
            system("cls||clear");
            string request = human.transaction_prepare();
            if(request.empty())
                cout << "Canceled" << endl;
            else
            {
                int i = request.size();
                WriteFile(hNamedPerformPipe, reinterpret_cast<char*>(&i), sizeof(int), NULL, NULL);
                WriteFile(hNamedPerformPipe, request.c_str(), i, NULL, NULL);

                SetEvent(hPerformEvent);

                WaitForSingleObject(hUserPerformEvent, INFINITE);
                ResetEvent(hUserPerformEvent);

                char buf[255];
                int b;

                ReadFile(hNamedPerformPipe, reinterpret_cast<char*>(&b), sizeof(int), NULL, NULL);
                ReadFile(hNamedPerformPipe, buf, b, NULL, NULL);


                buf[b] = '\0';
                if(!strncmp(buf,"Transaction is confirmed!", 25))
                    human.transaction_confirm(true);
                else
                    human.transaction_confirm(false);

                cout << buf << endl;
            }
            system("pause");
            break;
        }
        case 3:
        {
            system("cls||clear");
            string request = "Status";
            int i = request.size();

            WriteFile(hNamedPerformPipe, reinterpret_cast<char*>(&i), sizeof(int), NULL, NULL);
            WriteFile(hNamedPerformPipe, request.c_str(), i, NULL, NULL);

            SetEvent(hPerformEvent);

            WaitForSingleObject(hUserPerformEvent, INFINITE);
            ResetEvent(hUserPerformEvent);

            char buf[255];
            int b;

            ReadFile(hNamedPerformPipe, reinterpret_cast<char*>(&b), sizeof(int), NULL, NULL);
            ReadFile(hNamedPerformPipe, buf, b, NULL, NULL);

            buf[b] = '\0';

            cout<<buf<<endl;

            system("pause");
            break;
        }
        case 4:
        {
            system("cls||clear");

            string request = "Shutdown " + name;

            int i = request.size();

            WriteFile(hNamedPerformPipe, reinterpret_cast<char*>(&i), sizeof(int), NULL, NULL);
            WriteFile(hNamedPerformPipe, request.c_str(), i, NULL, NULL);

            SetEvent(hPerformEvent);

            exit(0);
        }
        }
    }

}

BOOL cProcessUnit::Exit(DWORD Event)
{
    string request = "Shutdown " + name;

    int i = request.size();
    SetEvent(hWriteConnectEvent);
    WaitForSingleObject(hUserWriteEvent,INFINITE);
    Sleep(50);

    hNamedWritePipe = CreateFile(hPipeWriteName, GENERIC_READ | GENERIC_WRITE,
                            0, NULL, OPEN_EXISTING, 0, NULL);

    WriteFile(hNamedWritePipe, reinterpret_cast<char*>(&i), sizeof(int), NULL, NULL);
    WriteFile(hNamedWritePipe, request.c_str(), i, NULL, NULL);

    SetEvent(hWriteEvent);
    Beep(750,500);
    return true;
}

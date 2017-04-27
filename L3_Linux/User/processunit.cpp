#include "processunit.h"

#ifdef _WIN32
LPSTR cProcessUnit::hPipeName = "\\\\.\\pipe\\$SecurityATmLine$";
HANDLE cProcessUnit::hNamedPipe;
HANDLE cProcessUnit::hMachineEvent;
HANDLE cProcessUnit::hUserEvent;
HANDLE cProcessUnit::hSemaphore;
#endif

#ifdef linux
int cProcessUnit::PipeDescript;
char cProcessUnit::PipeName[21] = "/tmp/SecurityATmLine";
int cProcessUnit::PipeDescriptConn;
char cProcessUnit::PipeNameConn[25] = "/tmp/SecurityATmLineConn";
bool cProcessUnit::User = false;
bool cProcessUnit::ATM = false;
sem_t* cProcessUnit::hSemaphore;
string cProcessUnit::name;
#endif

void cProcessUnit::StartProgram()
{
    cHuman human;


    cout << "Enter your name: ";
    cin >> name;

    cout << "Please waiting your quene!" << endl;

#ifdef _WIN32
    hSemaphore = OpenSemaphore(SEMAPHORE_ALL_ACCESS, false, "Semaphore");

    string comand("title User: " + human.getName());
    system(comand.c_str());

    WaitForSingleObject(hSemaphore, INFINITE);

    hNamedPipe = CreateFile(hPipeName, GENERIC_READ | GENERIC_WRITE,
                            0, NULL, OPEN_EXISTING, 0, NULL);

    hMachineEvent = OpenEvent(EVENT_ALL_ACCESS,FALSE,"hMachineEvent");

    hUserEvent = OpenEvent(EVENT_ALL_ACCESS,FALSE,"hUserEvent");
#endif

#ifdef linux
    pid_t pid = get_pid();
    PipeDescriptConn = open(PipeNameConn, O_RDWR);
    int size = name.size();
    write(PipeDescriptConn, reinterpret_cast<char*>(&size), sizeof(int));
    write(PipeDescriptConn, name.c_str(), size);

    close(PipeDescriptConn);
    kill(pid,SIGUSR2);

    signal(SIGINT,close_signal);

    hSemaphore = sem_open("Semaphore",0);
    sem_wait(hSemaphore);
    signal(SIGUSR2,atm_signal);
    PipeDescript = open(PipeName, O_RDWR);

#endif

    while(true)
    {
        system("cls||clear");
        cout << "Account menu [" << name << "]:" << endl;
        cout << "1.State of account" << endl;
        cout << "2.Withdraw money" << endl;
        cout << "3.Admin: ATM status" << endl;
        cout << "4.Finish work" << endl;

        int menu;

        while(!(cin >> menu) || menu < 1 || menu > 4)
        {
            cout << "Please input correct value!\n";
            cin.clear();
#ifdef _WIN32
            cin.ignore(cin.rdbuf()->in_avail());
#endif
#ifdef linux
	    cin.ignore(numeric_limits<streamsize>::max(), '\n');
#endif
        }

        switch (menu)
        {
        case 1:
        {
            system("cls||clear");
            cout << "You have: " << human.moneyAmount() << " BYN" << endl;
#ifdef _WIN32
                system("pause");
#endif

#ifdef linux
                press_any_key();
#endif
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
#ifdef _WIN32
                WriteFile(hNamedPipe, reinterpret_cast<char*>(&i), sizeof(int), NULL, NULL);
                WriteFile(hNamedPipe, request.c_str(), i, NULL, NULL);

                SetEvent(hMachineEvent);

                WaitForSingleObject(hUserEvent, INFINITE);
                ResetEvent(hUserEvent);
#endif

#ifdef linux
                write(PipeDescript, reinterpret_cast<char*>(&i), sizeof(int));

                write(PipeDescript, request.c_str(), i);

                kill(pid,SIGUSR1);

                while(!ATM);
                ATM = false;
#endif
                char buf[255];
                int b;

#ifdef _WIN32
                ReadFile(hNamedPipe, reinterpret_cast<char*>(&b), sizeof(int), NULL, NULL);
                ReadFile(hNamedPipe, buf, b, NULL, NULL);
#endif
#ifdef linux
                read(PipeDescript, reinterpret_cast<char*>(&b), sizeof(int));
                read(PipeDescript, buf, b);
#endif

                buf[b] = '\0';
                if(!strncmp(buf,"Transaction is confirmed!", 25))
                    human.transaction_confirm(true);
                else
                    human.transaction_confirm(false);

                cout << buf << endl;
            }
#ifdef _WIN32
                system("pause");
#endif

#ifdef linux
                press_any_key();
#endif
            break;
        }
        case 3:
        {
            system("cls||clear");
            string request = "Status";
            int i = request.size();

#ifdef _WIN32
            WriteFile(hNamedPipe, reinterpret_cast<char*>(&i), sizeof(int), NULL, NULL);
            WriteFile(hNamedPipe, request.c_str(), i, NULL, NULL);

            SetEvent(hMachineEvent);

            WaitForSingleObject(hUserEvent, INFINITE);
            ResetEvent(hUserEvent);
#endif

#ifdef linux
            write(PipeDescript, reinterpret_cast<char*>(&i), sizeof(int));

            write(PipeDescript, request.c_str(), i);

            kill(pid,SIGUSR1);

            while(!ATM);
            ATM = false;
#endif

            char buf[255];
            int b;

#ifdef _WIN32
            ReadFile(hNamedPipe, reinterpret_cast<char*>(&b), sizeof(int), NULL, NULL);
            ReadFile(hNamedPipe, buf, b, NULL, NULL);
#endif
#ifdef linux
            read(PipeDescript, reinterpret_cast<char*>(&b), sizeof(int));
            read(PipeDescript, buf, b);
#endif

            buf[b] = '\0';

            cout<<buf<<endl;

#ifdef _WIN32
            system("pause");
#endif

#ifdef linux
            press_any_key();
#endif
            break;
        }
        case 4:
        {
            system("cls||clear");

            string request = "Shutdown " + name;

            int i = request.size();
#ifdef _WIN32
            WriteFile(hNamedPipe, reinterpret_cast<char*>(&i), sizeof(int), NULL, NULL);
            WriteFile(hNamedPipe, request.c_str(), i, NULL, NULL);

            SetEvent(hMachineEvent);
#endif

#ifdef linux
            write(PipeDescript, reinterpret_cast<char*>(&i), sizeof(int));
            write(PipeDescript, request.c_str(), i);

            kill(pid,SIGUSR1);
            sem_post(hSemaphore);
#endif
            exit(0);
        }
        }
    }

}
#ifdef linux
void cProcessUnit::atm_signal(int s)
{
    ATM = true;
    signal(SIGUSR2,atm_signal);
}

int cProcessUnit::get_pid()
{
    sleep(3);
    char buf[128] = "/bin/pidof CashMachine";
    char* p = &buf[0];


    FILE* f = popen(buf, "r");
    if ( !f ) {
        perror("popen():");
        return -1;
    }
    int i = 0;
    while ( !feof(f) ) {
        *p++ = fgetc(f);
        i++;
    }
    pid_t pid;
    buf[i]='\0';
    pid = atoi(buf);

    return pid;
}

void cProcessUnit::press_any_key()
{
    cout << "please press ENTER....\0";
    cin.get();
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

void cProcessUnit::close_signal(int s)
{

    string request = "Shutdown " + name;

    size_t i;
    i = request.size();

    pid_t pid = get_pid();
    PipeDescriptConn = open(PipeNameConn, O_RDWR);

    write(PipeDescriptConn, reinterpret_cast<char*>(&i), sizeof(int));
    write(PipeDescriptConn, request.c_str(), i);

    kill(pid,SIGUSR2);
   //sem_post(hSemaphore);

    cout << "Goodbye" << endl;

    exit(0);
}
#endif

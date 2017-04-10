#include "processunit.h"

#ifdef _WIN32
LPSTR ProcessUnit::PipeName = "\\\\.\\pipe\\$SecurityATmLine$";
HANDLE ProcessUnit::hNamedPipe;
HANDLE ProcessUnit::UserEvent;
LPSTR ProcessUnit::UserEventName = "User";
HANDLE ProcessUnit::ATmEvent;
LPSTR ProcessUnit::ATmEventName = "ATM";
#endif

#ifdef linux
int ProcessUnit::PipeDescript;
char ProcessUnit::PipeName[21] = "/tmp/SecurityATmLine";
bool ProcessUnit::User = false;
bool ProcessUnit::ATM = false;
#endif


void ProcessUnit::START(int argc, char *argv[])
{

    if(argc == 1)
    {
#ifdef _WIN32
        STARTUPINFO si;
        PROCESS_INFORMATION pi;
        GetStartupInfo(&si);

        CreateProcess(NULL, (LPSTR)("L1.exe start_cashmashine"), NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi);

        UserEvent = CreateEvent(NULL,TRUE,FALSE,UserEventName);
        ATmEvent = CreateEvent(NULL,TRUE,FALSE,ATmEventName);

        hNamedPipe = CreateNamedPipe(PipeName,PIPE_ACCESS_DUPLEX,
                                     PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT,
                                     PIPE_UNLIMITED_INSTANCES,
                                     512,512,5000, NULL);

        ConnectNamedPipe(hNamedPipe, NULL);
#endif

#ifdef linux

    	pid_t pid;
    	pid = fork();

    	if(pid > 0)
    	{

    	}
    	if(pid == 0)
    	{
        	execl("script.sh",NULL,NULL); 
    	}


        unlink(PipeName);
        if((mkfifo(PipeName,0777)) == -1)
		cout << "Cannot create file\n";	
	
        PipeDescript = open(PipeName,O_RDWR);
	if (PipeDescript < 0)
	{
		cout<< "Cannot open file\n";
		cout << strerror(errno) <<endl;
		exit (1);
	}	
        signal(SIGUSR2,atm_signal);

	cout << "Please wait for boot system...." << endl;
        pid=get_pid();
#endif

        Human Man;

#ifdef _WIN32
        string comand("title User: " + Man.getName());
        system(comand.c_str());
#endif
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

                cout << "You have: " << Man.moneyAmount() << " BYN" << endl;

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

                string request = Man.transaction_prepare();

                if(request.empty())
                    cout << "Canceled" << endl;
                else
                {
                    int i = request.size();

#ifdef _WIN32
                    WriteFile(hNamedPipe, reinterpret_cast<char*>(&i), sizeof(int), NULL, NULL);
                    WriteFile(hNamedPipe, request.c_str(), i, NULL, NULL);

                    SetEvent(UserEvent);

                    WaitForSingleObject(ATmEvent, INFINITE);
                    ResetEvent(ATmEvent);
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
                        Man.transaction_confirm(true);
                    else
                        Man.transaction_confirm(false);

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

                SetEvent(UserEvent);

                WaitForSingleObject(ATmEvent, INFINITE);
                ResetEvent(ATmEvent);
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

                cout << buf << endl;

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

                string request = "Shutdown";

                int i = request.size();

#ifdef _WIN32
                WriteFile(hNamedPipe, reinterpret_cast<char*>(&i), sizeof(int), NULL, NULL);
                WriteFile(hNamedPipe, request.c_str(), i, NULL, NULL);

                SetEvent(UserEvent);

#endif

#ifdef linux
                write(PipeDescript, reinterpret_cast<char*>(&i), sizeof(int));
                write(PipeDescript, request.c_str(), i);

                kill(pid,SIGUSR1);
                
#endif
                return;
            }
            }
        }
    }
    else
        if(argc >= 2)
        {

#ifdef _WIN32
            UserEvent = OpenEvent(EVENT_ALL_ACCESS,FALSE,UserEventName);
            ATmEvent = OpenEvent(EVENT_ALL_ACCESS,FALSE,ATmEventName);


            hNamedPipe = CreateFile(PipeName, GENERIC_READ | GENERIC_WRITE,
                                    0, NULL, OPEN_EXISTING, 0, NULL);

            system("title ATM");
#endif

#ifdef linux
pid_t pid;
pid=get_pid();
            PipeDescript = open(PipeName, O_RDWR);
		
            signal(SIGUSR1,user_signal);
		
#endif
            CashMachine Machine;

            

            while(true)
            {
#ifdef _WIN32
                WaitForSingleObject(UserEvent, INFINITE);
                ResetEvent(UserEvent);
#endif

#ifdef linux
                while(!User);
                User = false;
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

                if(!strncmp(buf,"User:", 5))
                {
                    string answer = Machine.transaction_request(string(buf));
                    cout<<Machine.status()<<endl;

                    int i = answer.size();

#ifdef _WIN32
                    WriteFile(hNamedPipe, reinterpret_cast<char*>(&i), sizeof(int), NULL, NULL);
                    WriteFile(hNamedPipe, answer.c_str(), i, NULL, NULL);

                    SetEvent(ATmEvent);
#endif

#ifdef linux
                    write(PipeDescript, reinterpret_cast<char*>(&i), sizeof(int));
                    write(PipeDescript, answer.c_str(), i);

                    kill(pid, SIGUSR2);
#endif
                }
                else
                    if(!strncmp(buf,"Status", 6))
                    {
                        string answer = Machine.status();

                        int i = answer.size();

#ifdef _WIN32
                        WriteFile(hNamedPipe, reinterpret_cast<char*>(&i), sizeof(int), NULL, NULL);
                        WriteFile(hNamedPipe, answer.c_str(), i, NULL, NULL);

                        SetEvent(ATmEvent);
#endif

#ifdef linux
                        write(PipeDescript, reinterpret_cast<char*>(&i), sizeof(int));
                        write(PipeDescript, answer.c_str(), i);

                    kill(pid, SIGUSR2);
#endif
                    }
                    else
                        if(!strncmp(buf,"Shutdown", 8))
                        {
                            Machine.save_state();
                            return;
                        }
            }
        }
}

#ifdef linux
void ProcessUnit::user_signal(int s)
{
    User = true;
    signal(SIGUSR1,user_signal);
}

void ProcessUnit::atm_signal(int s)
{
    ATM = true;
    signal(SIGUSR2,atm_signal);
}

int ProcessUnit::get_pid()
{
    sleep(3);
    char buf[128] = "/bin/pidof L1";
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
    char temp[20];
    int k = 0, j = 0;

    while(true)
    {
        j=0;
        while(buf[k] != ' ')
        {
            temp[j] = buf[k];
            k++;
            j++;
        }
        temp[j] = '\0';
        pid = atoi(temp);
        if(pid != getpid()) break;
        k++;
    }
    return pid;
}

void ProcessUnit::press_any_key()
{
	cout << "please press ENTER....\0";
	cin.get();
	cin.clear();
	cin.ignore(numeric_limits<streamsize>::max(), '\n');
}
#endif


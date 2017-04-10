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
sem_t* cProcessUnit::hSemaphore;
bool cProcessUnit::User = false;
bool cProcessUnit::ATM = false;
pid_t cProcessUnit::pid;
struct sigaction cProcessUnit::act;
#endif

void cProcessUnit::StartProgram()
{
#ifdef _WIN32
    system("title CashMachine");

    hNamedPipe = CreateNamedPipe(hPipeName,PIPE_ACCESS_DUPLEX,
                                 PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT,
                                 PIPE_UNLIMITED_INSTANCES,
                                 512,512,5000, NULL);


    hMachineEvent = CreateEvent(NULL,TRUE,FALSE,"hMachineEvent");
    hUserEvent = CreateEvent(NULL,TRUE,FALSE,"hUserEvent");

    hSemaphore = CreateSemaphore(NULL,1,1,"Semaphore");

    ConnectNamedPipe(hNamedPipe,NULL);
#endif

#ifdef linux
    sem_unlink ("Semaphore");
    hSemaphore = sem_open ("Semaphore", O_CREAT , 0777, 1);


    unlink(PipeName);
    mkfifo(PipeName,0777);

    PipeDescript = open(PipeName,O_RDWR);


    memset(&act, 0, sizeof(act));
    act.sa_sigaction = user_signal;
    sigset_t set;
    sigemptyset(&set);
    sigaddset(&set, SIGUSR1);
    act.sa_mask = set;
    act.sa_flags = SA_SIGINFO;
    sigaction(SIGUSR1, &act, 0);


    system("clear");
#endif

    cMachine machine;

    while(true)
    {
#ifdef _WIN32
        WaitForSingleObject(hMachineEvent, INFINITE);
        ResetEvent(hMachineEvent);
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
            string answer = machine.transaction_request(string(buf));
            cout<<machine.status()<<endl;
            cout << "**************************"<<endl;

            int i = answer.size();
#ifdef _WIN32
            WriteFile(hNamedPipe, reinterpret_cast<char*>(&i), sizeof(int), NULL, NULL);
            WriteFile(hNamedPipe, answer.c_str(), i, NULL, NULL);

            SetEvent(hUserEvent);
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
                string answer = machine.status();

                int i = answer.size();
                cout<<answer.c_str()<<endl;

#ifdef _WIN32
                WriteFile(hNamedPipe, reinterpret_cast<char*>(&i), sizeof(int), NULL, NULL);
                WriteFile(hNamedPipe, answer.c_str(), i, NULL, NULL);

                SetEvent(hUserEvent);
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
                    machine.save_state();
#ifdef _WIN32
                    DisconnectNamedPipe(hNamedPipe);
                    ReleaseSemaphore(hSemaphore,1,NULL);
                    ConnectNamedPipe(hNamedPipe,NULL);
#endif
                }

    }
}

#ifdef linux
void cProcessUnit::user_signal(int s, siginfo_t *info, void *context)
{
    User = true;
    pid = info->si_pid;
}
#endif






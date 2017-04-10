#ifndef PROCESSUNIT
#define PROCESSUNIT

#include <iostream>
#include <limits>
using namespace std;

#ifdef _WIN32
#include <process.h>
#include <Windows.h>
#include <string>
#endif

#ifdef linux
#include <sys/sem.h>
#include <sys/types.h>
#include <fcntl.h>          
#include <semaphore.h>
#include <unistd.h>
#include <cstdlib>
#include <sys/stat.h>
#include <signal.h>
#include <sys/signalfd.h>
#include <string.h>
#endif

#include <sstream>
#include "cashmachine.h"

class cProcessUnit
{

private:

#ifdef _WIN32
    static HANDLE hNamedPipe;
    static LPSTR hPipeName;
    static HANDLE hSemaphore;
    static HANDLE hMachineEvent;
    static HANDLE hUserEvent;
#endif

#ifdef linux
    static int PipeDescript;
    static char PipeName[21];
    static sem_t* hSemaphore;
    static bool User;
    static bool ATM;
    static struct sigaction act;
    static pid_t pid;
#endif

public:
    static void StartProgram();

#ifdef linux
    static void user_signal(int s, siginfo_t *info, void *context);
#endif
};

#endif // PROCESSUNIT


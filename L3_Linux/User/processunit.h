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
#include <string.h>
#include <fstream>
#endif

#include <sstream>
#include "user.h"

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
    static int PipeDescriptConn;
    static char PipeNameConn[25];
    static bool User;
    static bool ATM;
    static string name;
#endif

public:
    static void StartProgram();
#ifdef linux
    static void atm_signal(int s);
    static void close_signal(int s);
    static int get_pid();
    static void press_any_key();
#endif
};

#endif // PROCESSUNIT


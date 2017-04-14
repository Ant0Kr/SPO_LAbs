#ifndef PROCESSUNIT
#define PROCESSUNIT

#include <iostream>
#include <limits>
using namespace std;

#include <process.h>
#include <Windows.h>
#include <string>

#include <sstream>
#include "user.h"
class cProcessUnit
{

private:

    static HANDLE hNamedWritePipe;
    static LPSTR hPipeWriteName;
    static HANDLE hNamedPerformPipe;
    static LPSTR hPipePerformName;

    static HANDLE hPerformSemaphore;
    static HANDLE hWriteSemaphore;

    static HANDLE hWriteEvent;
    static HANDLE hPerformEvent;
    static HANDLE hUserPerformEvent;
    static HANDLE hUserWriteEvent;
    static HANDLE hWriteConnectEvent;
    static HANDLE hPerformConnectEvent;
    static string name;

public:
    static void StartProgram();
    static BOOL Exit(DWORD Event);
};

#endif // PROCESSUNIT


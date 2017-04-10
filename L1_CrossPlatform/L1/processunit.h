#ifndef PROCESSUNIT_H
#define PROCESSUNIT_H

#include <iostream>
#include <limits>
using namespace std;

#ifdef _WIN32
#include <process.h>
#include <Windows.h>
#include <string>
#endif

#ifdef linux
#include <unistd.h>
#include <cstdlib>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>
#include <string.h>
#include <errno.h>
#endif

#include <sstream>
#include "human.h"
#include "cashmachine.h"

class ProcessUnit
{
private:
    ProcessUnit(){}

#ifdef _WIN32
    static HANDLE hNamedPipe;
    static LPSTR PipeName;

    static HANDLE UserEvent;
    static LPSTR UserEventName;

    static HANDLE ATmEvent;
    static LPSTR ATmEventName;
#endif

#ifdef linux
    static int PipeDescript;
    static char PipeName[21];

public:
    static bool User;
    static bool ATM;
#endif

public:
    static void START(int argc, char *argv[]);

#ifdef linux
    static void user_signal(int s);
    static void atm_signal(int s);
    static int get_pid();
    static void press_any_key();
#endif
};

#endif // PROCESSUNIT_H

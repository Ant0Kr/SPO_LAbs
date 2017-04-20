#ifndef PROJECT_H
#define PROJECT_H


#include<iostream>
using namespace std;

#ifdef _WIN32
#include <process.h>
#include <windows.h>
#include <conio.h>
#include <time.h>
#endif

#ifdef linux
#include <sys/sem.h>
#include <sys/types.h>
#include <fcntl.h>
#include <semaphore.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <ncurses.h>
#include <stdlib.h>
#include <sys/time.h>
#endif


#include <string>


class Project
{
public:
    Project(int writePause = 1000, int emersionPause = 5000);

#ifdef _WIN32           //поток
    static DWORD WINAPI ThreadRoutine(LPVOID lpParam);   //LPVOID указатель на информацию полученную от родителя
#endif
#ifdef linux
    static void* ThreadRoutine(void *arg);
#endif

    void START();

private:
    struct ThreadData
    {
        int ThreadId;
        int SleepTime;
    };

    int writePause;
    int emersionPause;
};

#endif // PROJECT_H

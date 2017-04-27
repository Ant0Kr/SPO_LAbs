#ifndef CPROCESSING
#define CPROCESSING

#include<iostream>
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
#include <signal.h>
#include <aio.h>
#include <errno.h>
#include <stddef.h>
#include <sys/stat.h>
#include <dlfcn.h>
using namespace std;

class cProcessing{
    
    struct readStruct{     //struct for reader Thread
        int counter;
        char **argv;
        char *buf;
        bool *readFlag;
        bool *writeFlag;
        bool *endFlag;
    };
    
    struct writeStruct{    //strucat for writter Thread
        char *buf;
        bool *readFlag;
        bool *writeFlag;
        bool *stopFlag;
        bool *endFlag;
    };
    
private:
    readStruct readS;
    writeStruct writeS;
    
public:
    void init(int argc, char* argv[]);
};

#endif // CPROCESSING


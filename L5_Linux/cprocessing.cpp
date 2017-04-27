#include "cprocessing.h"

#define bufSize 10240

typedef void * (*Type)(void*);                    //init own type

void cProcessing::init(int argc, char *argv[]){

    char * buf = (char*)malloc(bufSize);          //buff for transport data
    bool * writeFlag = new bool;                  //flags for synchronize threads
    *writeFlag = false;
    bool * readFlag = new bool;
    *readFlag = true;
    bool * endFlag = new bool;
    *endFlag = false;

    void * handle;
    handle = dlopen("./libmylib.so", RTLD_LAZY);  //open lib
    if (!handle) {
        fprintf(stderr, "%s\n", dlerror());
        exit(EXIT_FAILURE);
    }

    Type reader = (Type)dlsym(handle, "readerThreadRoutine");   //get descriptor of methods from lib
    Type writter = (Type)dlsym(handle, "writterThreadRoutine");

    readS.counter = argc-1;                                    //init reader and writter struct
    readS.argv = (char**)malloc(readS.counter*sizeof(char*));
    readS.buf = buf;
    readS.readFlag = readFlag;
    readS.writeFlag = writeFlag;
    readS.endFlag = endFlag;
    for(int i = 0;i<argc-1;i++){                //get name of files
        readS.argv[i] = argv[i+1];
    }

    writeS.buf = buf;
    writeS.readFlag = readFlag;
    writeS.writeFlag = writeFlag;
    writeS.stopFlag = new bool;
    *writeS.stopFlag = false;
    writeS.endFlag = endFlag;

    pthread_t writterThread;           //create Threads
    pthread_t readerThread;
    pthread_create(&writterThread, NULL, writter,&writeS);
    pthread_create(&readerThread, NULL, reader, &readS);


    while(*writeS.stopFlag == false)  //while write thread work, main wait.
        sleep(1);
    cout<<"Program exit."<<endl;

    dlclose(handle);

    return ;
}



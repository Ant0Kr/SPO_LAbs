#include "project.h"

Project::Project(char *directory, char *outputFileName)
{
    if(directory == nullptr)
    {
        GetCurrentDirectory(MAX_PATH, this->directory);
        strcat(this->directory,"\\");
    }
    else
        strcpy(this->directory, directory);

    this->outputFileName = new char[sizeof(outputFileName)];
    strcpy(this->outputFileName, outputFileName);
}

void Project::START()
{
    HINSTANCE hLib = LoadLibrary("L5Dll.dll");
    if(!hLib)
    {
        cout << "To work we need L5Dll.dll!\n";
        exit (0);
    }

    LPFNDLLinit init = (LPFNDLLinit)GetProcAddress(hLib,TEXT("init"));
    HANDLE WriteOffEvent = init(directory,outputFileName);

    if(WriteOffEvent == 0)
    {
        cout << "Error of input information!\n";
        exit (0);
    }

    LPTHREAD_START_ROUTINE  reader = (LPTHREAD_START_ROUTINE)GetProcAddress(hLib,TEXT("ThreadReader"));
    LPTHREAD_START_ROUTINE  writer = (LPTHREAD_START_ROUTINE)GetProcAddress(hLib,TEXT("ThreadWriter"));

    HANDLE threadReader = CreateThread(NULL,0,reader,NULL,0,0);
    HANDLE threadWriter = CreateThread(NULL,0,writer,NULL,0,0);

    WaitForSingleObject(threadReader, INFINITE);
    WaitForSingleObject(WriteOffEvent, INFINITE);

    TerminateThread(threadWriter,0);

    cout << "Combining completed successfully!\n";
    FreeLibrary(hLib);
    exit(0);
}


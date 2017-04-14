#include "processunit.h"

#include <windows.h>
#include <tlhelp32.h>
#include <iterator>

HANDLE cProcessUnit::hNamedWritePipe;
LPSTR cProcessUnit::hPipeWriteName = "\\\\.\\pipe\\$SecurityATmWriteLine$";
HANDLE cProcessUnit::hNamedPerformPipe;
LPSTR cProcessUnit:: hPipePerformName = "\\\\.\\pipe\\$SecurityATmPerformLine$";

HANDLE cProcessUnit::hPerformEvent;
HANDLE cProcessUnit::hWriteEvent;
HANDLE cProcessUnit::hUserPerformEvent;
HANDLE cProcessUnit::hUserWriteEvent;
HANDLE cProcessUnit::hWriteConnectEvent;
HANDLE cProcessUnit::hPerformConnectEvent;

HANDLE cProcessUnit::hPerformSemaphore;
HANDLE cProcessUnit::hWriteSemaphore;
string cProcessUnit::name;

void cProcessUnit::StartProgram()
{
    system("title CashMachine");


    list <string> my_list;

    hNamedWritePipe = CreateNamedPipe(hPipeWriteName,PIPE_ACCESS_DUPLEX,
                                      PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT,
                                      PIPE_UNLIMITED_INSTANCES,
                                      512,512,5000, NULL);
    hNamedPerformPipe = CreateNamedPipe(hPipePerformName,PIPE_ACCESS_DUPLEX,
                                        PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT,
                                        PIPE_UNLIMITED_INSTANCES,
                                        512,512,5000, NULL);


    hPerformEvent = CreateEvent(NULL,TRUE,FALSE,"hPerformEvent");
    hWriteEvent = CreateEvent(NULL,TRUE,FALSE,"hWriteEvent");
    hUserPerformEvent = CreateEvent(NULL,TRUE,FALSE,"hUserPerformEvent");
    hUserWriteEvent = CreateEvent(NULL,TRUE,FALSE,"hUserWriteEvent");
    hWriteConnectEvent = CreateEvent(NULL,TRUE,FALSE,"hWriteConnectEvent");;
    hPerformConnectEvent = CreateEvent(NULL,TRUE,FALSE,"hPerformConnectEvent");

    hPerformSemaphore = CreateSemaphore(NULL,1,1,"PerformSemaphore");
    hWriteSemaphore = CreateSemaphore(NULL,1,1,"WriteSemaphore");

    cMachine machine;

    while(true)
    {
        system("cls");
        copy(my_list.begin(), my_list.end(), ostream_iterator<string>(cout," "));

        DWORD dwWaitResult = WaitForSingleObject(hWriteConnectEvent,1);
        ResetEvent(hWriteConnectEvent);
        if(dwWaitResult == WAIT_OBJECT_0 )
        {
            DisconnectNamedPipe(hNamedWritePipe);
            SetEvent(hUserWriteEvent);
            ConnectNamedPipe(hNamedWritePipe,NULL);

        }

        dwWaitResult = WaitForSingleObject(hPerformConnectEvent,1);
        ResetEvent(hPerformConnectEvent);
        if(dwWaitResult == WAIT_OBJECT_0 )
        {
            DisconnectNamedPipe(hNamedPerformPipe);
            SetEvent(hUserPerformEvent);
            ConnectNamedPipe(hNamedPerformPipe,NULL);
        }


        dwWaitResult = WaitForSingleObject(hPerformEvent,1);
        ResetEvent(hPerformEvent);

        if(dwWaitResult == WAIT_OBJECT_0 )
        {
            char buf[255];
            int b;

            ReadFile(hNamedPerformPipe, reinterpret_cast<char*>(&b), sizeof(int), NULL, NULL);
            ReadFile(hNamedPerformPipe, buf, b, NULL, NULL);

            buf[b] = '\0';

            if(!strncmp(buf,"User:", 5))
            {
                string answer = machine.transaction_request(string(buf));

                int i = answer.size();

                WriteFile(hNamedPerformPipe, reinterpret_cast<char*>(&i), sizeof(int), NULL, NULL);
                WriteFile(hNamedPerformPipe, answer.c_str(), i, NULL, NULL);
                SetEvent(hUserPerformEvent);
            }
            else
                if(!strncmp(buf,"Status", 6))
                {

                    string answer = machine.status();

                    int i = answer.size();
                    WriteFile(hNamedPerformPipe, reinterpret_cast<char*>(&i), sizeof(int), NULL, NULL);
                    WriteFile(hNamedPerformPipe, answer.c_str(), i, NULL, NULL);

                    SetEvent(hUserPerformEvent);
                }
                else
                    if(!strncmp(buf,"Shutdown", 8))
                    {
                        my_list.pop_front();
                        machine.save_state();
                        ReleaseSemaphore(hPerformSemaphore,1,NULL);
                    }

        }

        dwWaitResult = WaitForSingleObject(hWriteEvent,1);
        ResetEvent(hWriteEvent);

        if(dwWaitResult == WAIT_OBJECT_0 )
        {
            char buf[255];
            int b;
            ReadFile(hNamedWritePipe, reinterpret_cast<char*>(&b), sizeof(int), NULL, NULL);
            ReadFile(hNamedWritePipe, buf, b, NULL, NULL);

            buf[b] = '\0';
            if(!strncmp(buf,"Shutdown",8))
            {
                machine.save_state();
                name = string(buf+9);
                my_list.remove_if(check);

            }
            else
            {
                my_list.push_back(buf);
                ReleaseSemaphore(hWriteSemaphore,1,NULL);
            }

        }
    }




}

bool cProcessUnit::check(string str)
{
    if(strstr(str.c_str(),name.c_str()))
        return true;
    else
        return false;
}








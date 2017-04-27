#include "l5dll.h"

extern "C" __declspec(dllexport) DWORD ThreadReader(LPVOID lpParam)
{
    list<string>::iterator iterFileList = readerInfo.fileList.begin();

    OVERLAPPED overlapped = {0};
    overlapped.hEvent = readerInfo.info->ReadOffEvent;

    while(iterFileList != readerInfo.fileList.end())
    {
        HANDLE fileHandle = CreateFile((readerInfo.info->CurDir + *iterFileList).c_str(),GENERIC_READ,
                                       0,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);

        overlapped.Offset = 0;
        readerInfo.info->ReadBytes = 0;

        cout << "Reading file " << *iterFileList << " ..." << endl;

        DWORD fileSize = GetFileSize(fileHandle, NULL);

        while(true)
        {
            WaitForSingleObject(readerInfo.info->WriteOffEvent,INFINITE);

            overlapped.Offset += readerInfo.info->ReadBytes;
            if(overlapped.Offset == fileSize) break;

            ResetEvent(readerInfo.info->WriteOffEvent);

            ReadFile(fileHandle,readerInfo.info->buffer,readerInfo.info->sizeBuffer,
                     &readerInfo.info->ReadBytes,&overlapped);

        }

        iterFileList++;
    }

    return 0;
}

extern "C" __declspec(dllexport) DWORD ThreadWriter(LPVOID lpParam)
{
    OVERLAPPED overlapped = {0};
    overlapped.hEvent = writerInfo.info->WriteOffEvent;

    while(true)
    {
        WaitForSingleObject(writerInfo.info->ReadOffEvent,INFINITE);
        ResetEvent(writerInfo.info->ReadOffEvent);

        HANDLE fileHandle = CreateFile((writerInfo.info->CurDir + writerInfo.outputFile).c_str(),GENERIC_WRITE,
                                       0,NULL,OPEN_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);

        overlapped.Offset += writerInfo.info->WriteBytes;

        WriteFile(fileHandle,writerInfo.info->buffer,writerInfo.info->ReadBytes,
                  &writerInfo.info->WriteBytes,&overlapped);

        CloseHandle(fileHandle);
    }

    return 0;
}

bool listCompare(string str1, string str2)
{
    if(str1.size() != str2.size())
        return str1.size() < str2.size();
    return strcmp(str1.c_str(), str2.c_str()) < 0;
}

bool getTxtFiles(string directory, list<string> *fileList)
{
    WIN32_FIND_DATA dataFile;
    HANDLE file;

    file = FindFirstFile((directory + "*.txt").c_str(), &dataFile);

    if(file != INVALID_HANDLE_VALUE)
        do{
            fileList->push_back(dataFile.cFileName);
        }while(FindNextFile(file,&dataFile));
    else
        return false;

    FindClose(file);

    fileList->sort(listCompare);


    return true;
}

extern "C" __declspec(dllexport) HANDLE init(char *curDir, char *outputFile)
{
    commonInfo.buffer = new char[10240];
    commonInfo.sizeBuffer = 10240;
    commonInfo.CurDir = (string)curDir;
    commonInfo.ReadOffEvent = CreateEvent(NULL,TRUE,FALSE,NULL);
    commonInfo.WriteOffEvent = CreateEvent(NULL,TRUE,FALSE,NULL);
    commonInfo.ReadBytes = 0;
    commonInfo.WriteBytes = 0;

    writerInfo.outputFile = (string)outputFile;
    writerInfo.info = &commonInfo;

    if(!getTxtFiles(commonInfo.CurDir,&readerInfo.fileList))
       return 0;

    readerInfo.fileList.remove(writerInfo.outputFile);

    readerInfo.info = &commonInfo;

    remove((commonInfo.CurDir + writerInfo.outputFile).c_str());

    SetEvent(commonInfo.WriteOffEvent);

    return commonInfo.WriteOffEvent;
}

#ifndef PROJECT_H
#define PROJECT_H

#include <iostream>
using namespace std;
#include <Windows.h>

typedef HANDLE (CALLBACK* LPFNDLLinit)(char *,char *);

class Project
{
public:
    Project(char *directory = nullptr, char *outputFileName = "output.txt");
    void START();
private:
    char directory[MAX_PATH];
    char *outputFileName;
};

#endif // PROJECT_H

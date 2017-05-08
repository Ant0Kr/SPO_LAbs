#ifndef PROJECT_H
#define PROJECT_H

#include <iostream>
using namespace std;

#include <Windows.h>
#include <WindowsX.h>

class Project6
{   
    Project6(){}

public:
    static void* malloc(size_t size);
    static void* realloc(void* pointer, size_t newSize);
    static void free(void *pointer);
};

#endif // PROJECT_H

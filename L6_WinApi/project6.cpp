#include "project6.h"

void *Project6::malloc(size_t size)
{
    HLOCAL hMemUnit;
    void*  pointer;

    cout << "Alocation memory" << endl;

    hMemUnit = LocalAlloc(LHND,size);  // LHND - moveable and in all bytes writed zero

    if(hMemUnit == NULL)
    {
        cout << "Alocation memory failed, trying to defragmentation" << endl;

        LocalCompact(size); //try to defragmentation

        hMemUnit = LocalAlloc(LHND,size);

        if(hMemUnit != NULL)
        {
            cout << "Alocation memory completed" << endl;
            pointer = LocalLock(hMemUnit);
        }
        else
        {
            cout << "Alocation memory failed" << endl;
            pointer = nullptr;
        }
    }
    else
    {
        cout << "Alocation memory completed" << endl;
        pointer = LocalLock(hMemUnit);
    }

    return pointer;
}

void *Project6::realloc(void *pointer, size_t newSize)
{
    HLOCAL hMemUnit = LocalHandle(pointer);

    cout << "Realocation memory" << endl;

    LocalUnlock(hMemUnit);

    hMemUnit = LocalReAlloc(hMemUnit, newSize, LMEM_ZEROINIT);

    if(hMemUnit == NULL)
    {
        cout << "Realocation memory failed" << endl;
        pointer = nullptr;
    }
    else
    {
        cout << "Realocation memory completed" << endl;
        pointer = LocalLock(hMemUnit);
    }

    return pointer;
}

void Project6::free(void *pointer)
{
    HLOCAL hMemUnit = LocalHandle(pointer);

    LocalUnlock(hMemUnit);

    if(LocalFree(hMemUnit) == NULL)
        cout << "Memory free completed" << endl;
    else
        cout << "Memory free failed" << endl;
}

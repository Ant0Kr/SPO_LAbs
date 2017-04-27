#include "project.h"

int main(int argc, char *argv[])
{
    if(argc == 1)
    {
        Project project5;
        project5.START();
    }
    else if(argc == 2)
    {
        Project project5(argv[1]);
        project5.START();
    }else if(argc > 2)
    {
        Project project5(argv[1],argv[2]);
        project5.START();
    }

    return 0;
}


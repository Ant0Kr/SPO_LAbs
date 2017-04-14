#include <QCoreApplication>
#include "processunit.h"
#include <stdlib.h>
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    cProcessUnit::StartProgram();


    return a.exec();
}


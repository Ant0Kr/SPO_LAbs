#include "project.h"

Project::Project(int writePause, int emersionPause)
{
    this->writePause = writePause;
    this->emersionPause = emersionPause;
}

#ifdef _WIN32
DWORD WINAPI Project::ThreadRoutine(LPVOID lpParam)
{
    struct ThreadData Data = *((struct ThreadData*)lpParam);
    HANDLE hSemaphore = OpenSemaphore(SEMAPHORE_ALL_ACCESS, false, "Semaphore");

    while(true)
    {
        WaitForSingleObject(hSemaphore, INFINITE);

        if(Data.ThreadId == 1)
        {
            Sleep(Data.SleepTime);                 //задержка для вывода потоков
            cout<<"\nT[" << Data.ThreadId << "] "; //первый переносит вывод на новую строку
        }                                          //
        else
            cout<<"T[" << Data.ThreadId << "] ";

        ReleaseSemaphore(hSemaphore,1,NULL);
    }

    return 0;
}
#endif

#ifdef linux
void* Project::ThreadRoutine(void *arg)
{

    struct ThreadData Data = *((struct ThreadData*)arg);
    sem_t* hSemaphore = sem_open("Semaphore",0);

    while(true)
    {
        sem_wait(hSemaphore);

        if(Data.ThreadId == 1)
        {
            napms(Data.SleepTime);
            printw("\nT[%d] ",Data.ThreadId);
        }           
        else
            printw("T[%d] ",Data.ThreadId);

        sem_post(hSemaphore);
        napms(1);
    }
}
#endif

void Project::START()
{

#ifdef linux
    printf ("\[\e]0;ThReAdS aRe dAnciNg\a\]");

    initscr();              // инициализация
    cbreak();               // отключение буферизированного ввода (один символ в мометн времени)
    noecho();               // подавление эхо
    scrollok(stdscr, TRUE); // блокировка скрола терминала
    nodelay(stdscr, TRUE);  // при нем getch() неблокирующий
    curs_set(0);            // скрытие курсора
#endif

#ifdef _WIN32
    CreateSemaphore(NULL,1,1,"Semaphore");
#endif
#ifdef linux
    sem_unlink ("Semaphore");
    sem_open ("Semaphore", O_CREAT , 0777, 1);
#endif

    int threadCounter = 1;

    struct ThreadData *threadData;   //массив структур хранящий данные для потоков

#ifdef _WIN32
    HANDLE *ThreadHandles;        //массив дескрипторов потоков
#endif
#ifdef linux
    pthread_t *ThreadHandles;
#endif

    threadData = new ThreadData;    //создаем первую структуру для первого потока
    threadData[threadCounter - 1].ThreadId = threadCounter;
    threadData[threadCounter - 1].SleepTime = writePause;   //инициализация структуры

#ifdef _WIN32
    ThreadHandles = new HANDLE;       //выделение памяти под 1 дескриптор
    ThreadHandles[threadCounter - 1] = CreateThread(NULL,0,Project::ThreadRoutine,&threadData[threadCounter - 1],0,0);
    //создаем поток и запускаем его
#endif
#ifdef linux
    ThreadHandles = new pthread_t;
    pthread_create(&ThreadHandles[threadCounter - 1], NULL, ThreadRoutine, &threadData[threadCounter - 1]);
#endif

    char temp;   //* - +

    long long int start = clock();  //время в миллисекундах для автоматической генерации потока

    while (true) {

#ifdef _WIN32
        if(_kbhit())//проверка буфера(пуст/полон) return 1,если в буфере что то есть
        {
#endif

            temp = getch();//use т.к ввод без enter и нет echo(отображ на экран)
            if(temp == '+')
            {
                threadCounter++;

                threadData = (struct ThreadData*)realloc(threadData,threadCounter * sizeof(struct ThreadData));
                threadData[threadCounter - 1].ThreadId = threadCounter;//добаление элемента в массив структур
                threadData[threadCounter - 1].SleepTime = writePause;

#ifdef _WIN32
                //добавление нового потока
                ThreadHandles = (HANDLE*)realloc(ThreadHandles,threadCounter * sizeof(HANDLE));
                ThreadHandles[threadCounter-1] = CreateThread(NULL,0,Project::ThreadRoutine,&threadData[threadCounter - 1],0,0);
#endif
#ifdef linux
                ThreadHandles = (pthread_t*)realloc(ThreadHandles,threadCounter * sizeof(pthread_t));
                pthread_create(&ThreadHandles[threadCounter - 1], NULL, ThreadRoutine, &threadData[threadCounter - 1]);
#endif
            }
            if(temp == '-' && threadCounter > 1)
            {

#ifdef _WIN32
                TerminateThread(ThreadHandles[threadCounter - 1],0);//остановка работы потока
#endif
#ifdef linux
                pthread_cancel(ThreadHandles[threadCounter - 1]);
#endif

                threadCounter--;

#ifdef _WIN32
                ThreadHandles = (HANDLE*)realloc(ThreadHandles,threadCounter * sizeof(HANDLE));//удаление его из массива
#endif                                  //переносив массив в новую ппамять без удаляемого потока
#ifdef linux
                ThreadHandles = (pthread_t*)realloc(ThreadHandles,threadCounter * sizeof(pthread_t));
#endif

                threadData = (struct ThreadData*)realloc(threadData,threadCounter * sizeof(struct ThreadData));
                            //то же со структурой
            }
            if(temp == '*')
            {
#ifdef linux
                endwin();
#endif
                exit(0);
            }

#ifdef _WIN32
        }
#endif

#ifdef _WIN32
        string str = "title Time to emersion new Thread(ms): " + to_string(emersionPause - (clock() - start));
        system(str.c_str());
#endif

#ifdef _WIN32
        if((clock() - start) > emersionPause)
#endif
#ifdef linux
        if((clock() - start) / 1000 > emersionPause)
#endif
        {
            start = clock();

            threadCounter++;

            threadData = (struct ThreadData*)realloc(threadData,threadCounter * sizeof(struct ThreadData));
            threadData[threadCounter - 1].ThreadId = threadCounter;
            threadData[threadCounter - 1].SleepTime = writePause;

#ifdef _WIN32
            ThreadHandles = (HANDLE*)realloc(ThreadHandles,threadCounter * sizeof(HANDLE));
            ThreadHandles[threadCounter - 1] = CreateThread(NULL,0,Project::ThreadRoutine,&threadData[threadCounter - 1],0,0);
#endif
#ifdef linux
            ThreadHandles = (pthread_t*)realloc(ThreadHandles,threadCounter * sizeof(pthread_t));
            pthread_create(&ThreadHandles[threadCounter - 1], NULL, ThreadRoutine, &threadData[threadCounter - 1]);
#endif
        }
    }
}

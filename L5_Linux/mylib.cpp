#include <iostream>
using namespace std;
#include <sys/sem.h>
#include <sys/types.h>
#include <fcntl.h>
#include <semaphore.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <ncurses.h>
#include <stdlib.h>
#include <sys/time.h>
#include <signal.h>
#include <aio.h>
#include <errno.h>
#include <stddef.h>
#include <sys/stat.h>
#include <dlfcn.h>

#include <stdio.h>
#define bufSize 10240

struct readStruct{
    	
	int counter;
    	char **argv;
        char *buf;
        bool *readFlag;
        bool *writeFlag;
        bool *endFlag;
};
    
struct writeStruct{
        char *buf;
        bool *readFlag;
        bool *writeFlag;
        bool *stopFlag;
        bool *endFlag;
};

extern "C" void * writterThreadRoutine(void *arg){
    	
	aiocb cb;                                         //struct,that contain inform for AIO
        int offset = 0;				          
        writeStruct wStruct = *(writeStruct*)arg;         //get struct from main
        bool flag = true;    //first open of file(for clear file)
                                   
    	while(true)
    	{
        	if(*wStruct.writeFlag == true )   //let's write
        	{

		    if(flag == true){
		        cb.aio_fildes = open("outp.txt",O_WRONLY | O_TRUNC,0);
		        flag = false;
		    }else
			cb.aio_fildes = open("outp.txt",O_WRONLY,0);
		    
		    if(cb.aio_fildes == -1)
		    {
		        cout<<"Error of open file: outp.txt"<<endl;
		        return NULL;
		    }

		    int j = 0;
		    while(wStruct.buf[j]!='\0')  //search '\0' in buf(inform about and of write data)
		        j++;
	            
		    cb.aio_nbytes = j;           //sets quantity bytes for writing
		    cb.aio_offset = offset;      
		    cb.aio_buf = wStruct.buf;
		    offset+=j;                  //add offset

		    if(aio_write(&cb) == -1){
		        cout<<"Error of write!"<<endl;
		        return NULL;
		    }else{
		        while(aio_error(&cb) == EINPROGRESS)
		            sleep(0.1);
		    }

		    cout<<"Write data into file:outp.txt complete!"<<endl;
		    *wStruct.writeFlag = false;               //"deactivate" writeThread		    
	            *wStruct.readFlag = true;                 //"activate" work readThread
		    

		}

		if(*wStruct.endFlag == true && *wStruct.writeFlag == false){  //exit from writeThread
		    cout<<"Writter stop works!"<<endl;
		    *wStruct.stopFlag = true;
		    return NULL;
		}
	}
}

extern "C" void * readerThreadRoutine(void *arg){
    
    readStruct readS = *((readStruct*)arg);  //get readStruct from main
    aiocb cb;
    int i = 0;
    
    while(true)
    {

        if(i<readS.counter && *readS.readFlag == true){ //counter - quantity of files for reading

            //memset(&cb,0,sizeof(aiocb));

            cb.aio_fildes = open(readS.argv[i],O_RDONLY,0); //open file for reading
            if(cb.aio_fildes == -1)
            {
                cout<<"Error of open file:"<<readS.argv[i]<<endl;
                i++;
                continue;
            }else{
                cout<<"File:"<<readS.argv[i]<<" is opened!"<<endl;
            }

            struct stat st;            //get size of file.
            stat(readS.argv[i],&st);
            int size = st.st_size;
            int readData;	      //num of bytes for reading
            int offset = 0;           //offset for read file that more than buf

            while(size>0){

		if(*readS.readFlag == true){ //if false - wait for next reading                    
   		    
		    cb.aio_offset = offset;  
                    if(size>bufSize)
                    {
                        readData = bufSize;
                        size -= bufSize;
                        offset+=bufSize;
                    }
                    else{
                        readData = size;
                        size = 0;
                    }

                    cb.aio_nbytes = readData;
                    cb.aio_buf = readS.buf;

                    if(aio_read(&cb) == -1)
                    {
                        cout<<"Error of read file!"<<endl;
                        i++;
                        break;
                    }
                    else
                    {
                        while(aio_error(&cb) == EINPROGRESS){
                            sleep(0.1);
                        }

                        if( aio_return(&cb) != -1)
			    cout<<"Reading "<<readS.argv[i]<<" success!"<<endl;
                        else{
                            cout<<"Error of reading!"<<endl;
                            break;
                        }

                    }
	            
                    
		    readS.buf[readData] = '\0';//sets 0 how end of data for write.
		    
                    *readS.writeFlag = true;
                    *readS.readFlag = false;
                
		}
            }
            i++;

        }
        if(i == readS.counter){
            cout<<"Reader thread readed all files\nReader stop works."<<endl;
            *readS.endFlag = true;
            return NULL;
        }
    }
}

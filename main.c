#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include <errno.h>
#include <math.h>
#include <time.h>
#include <pthread.h>
#include "buffer.h"

BufferItem buffer [BUFFER_SIZE];

int InsertItem(BufferItem item){
    // insert item into buffer
    // return 0 if successful, otherwise
    // return -1 indicating an error condition
}

int RemoveItem(BufferItem *item){
    // remove an object from buffer
    // place it in item
    // return 0 if sucessful, otherwise
    // return -1 indicating an error condition
}

int main(int argc,char** argv)
{
//    1. get command line arguments argv[1], argv[2], argv[3]
    char* c = argv[1];
//    int sleepTime = *argv[1] - '0';
//    printf("sleep time: %d\n", sleepTime);
    printf("size of c: %ld\n", strlen(c));
    printf("contents of c: %s\n", c);
//    2. initialize buffer
//    2a. initialize mutex for buffer
//    3. create producer thread(s)
//    4. create consumer thread(s)
//    5. sleep
//    6. Exit

    return 0;
}

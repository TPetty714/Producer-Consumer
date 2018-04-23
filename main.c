#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include <errno.h>
#include <math.h>
#include <time.h>
#include <pthread.h>
#include <stdbool.h>
#include <string.h>
#include "buffer.h"

BufferItem buffer [BUFFER_SIZE];

void *Consumer(void *param);
int InsertItem(BufferItem item);
void *Producer(void *param);
int RemoveItem(BufferItem *item);

void *Consumer(void *param){
    BufferItem item;
    while(true){
//        sleep for a random period of time
//        sleep();
        if(RemoveItem(&item)){
            fprintf(stderr,"remove item error");
        }else{
            printf("consumer consumed %d\n", item);
        }
    }
}

int InsertItem(BufferItem item){
    // insert item into buffer
    // return 0 if successful, otherwise
    // return -1 indicating an error condition
    return 0;
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

void *Producer(void *param){
    BufferItem item;
    while(true){
//        sleep for a random period of time
//        sleep();
//        generate a random number
        item = rand();
        if(InsertItem(item)){
            fprintf(stderr, "report error");
        }else{
            printf("producer produced %d\n", item);
        }
    }
}


int RemoveItem(BufferItem *item){
    // remove an object from buffer
    // place it in item
    // return 0 if sucessful, otherwise
    // return -1 indicating an error condition
    return 0;
}

int Power(int base, int exp){

    if (exp == 0)
        return 1;
    else if (exp == 1)
        return base;
    else{
        for(int i = 1; i < exp; i++){

        }
    }

}

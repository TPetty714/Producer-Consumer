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
    printf("stuff to print");
    return 0;
}
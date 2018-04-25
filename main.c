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
#include <signal.h>
#include "buffer.h"

char* ShareMalloc(int);

BufferItem itemBuffer [BUFFER_SIZE];

pthread_t *producerThreads;
pthread_t *consumerThreads;
pthread_mutex_t buff_mutex;
pthread_cond_t empty;
pthread_cond_t full;
int exitProducers;
int exitConsumers;
int runTime = 0;
int numProdThreads = 0;
int numConsThreads = 0;

void Consumer(int conId);
int Exit(int);
void initConsumerThreads(int numThreads);
void initItemBuffer();
void initMutexAndCondVars();
void initProducerThreads(int numThreads);
int InsertItem(BufferItem item);
void Producer(int prodId);
int RemoveItem(BufferItem *item);
void ThreadExit(int);

int main(int argc,char** argv){
//    1. get command line arguments argv[1], argv[2], argv[3]
    char* c = argv[1];

    signal(SIGINT, ThreadExit);
//    get arguments
//    sleep time, number producers, number consumers
    if (argc == 4){
        sscanf(argv[1],"%d", &runTime);
        runTime = strtol(argv[1], &c, 10);
        sscanf(argv[2], "%d", &numProdThreads);
        numProdThreads = strtol(argv[2], &c, 10);
        sscanf(argv[3], "%d", &numConsThreads);
        numConsThreads = strtol(argv[3], &c, 10);
    }else{
        printf("using default values\n");
        runTime = 1;
        numProdThreads = 5;
        numConsThreads = 5;
    }


//    2. initialize buffer
    initItemBuffer();
//    2a. initialize mutex and condVars
    initMutexAndCondVars();

//    3. create producer thread(s)
    initProducerThreads(numProdThreads);

//    4. create consumer thread(s)
    initConsumerThreads(numConsThreads);

//    5. sleep
    printf("main sleeping\n");
    sleep(runTime);
    printf("main DONE sleeping Raising Quit Alarm\n");
//        raise signal for threads to quit
    raise(SIGINT);
//    }

//    6. Exit

//    printf("Exiting\n");
    Exit(1);
//    sleep(runTime);
    return 0;
}
void ThreadExit(int sig){
    signal(SIGINT, ThreadExit);
    printf("thread exit\n");
    exitConsumers = 1;
    exitProducers = 1;
}

void Consumer(int conId) {
    BufferItem item = 0;
    printf("Consumer %d!\n", conId);
//    int id = conId;
//    signal(SIGINT, ThreadExit(id));
    int randSleep;
    while(1) {
        randSleep = rand()%runTime;
        if (exitConsumers == 1)
            return;
        while (pthread_mutex_lock(&buff_mutex) != 0)
            fprintf(stderr, "ERROR on pthread_mutex_lock. consumerID: %d, errno:%d ", conId + 1, errno);
//        while the item removed is not successful
//        things to consume do not exist
        if(RemoveItem(&item) == -1){
            printf("Empty Buffer!\n");
//            signal that the itemBuffer is empty
//            pthread_cond_signal(&empty);
            pthread_cond_broadcast(&empty);
//            pthread_cond_wait(&full, &buff_mutex);
        } else {
            printf("Consumer %d Consumed %d\n", conId, item);
        }
        while (pthread_mutex_unlock(&buff_mutex) != 0)
            fprintf(stderr, "ERROR on pthread_mutex_unlock. consumerID: %d, errno:%d ", conId + 1, errno);
        sleep(randSleep);
        fprintf(stdout, "Consumer %d done sleeping: %d\n", conId, randSleep);
        fflush(stdout);
    }
}

int Exit(int sig) {
    printf("Exiting\n");
//    signal(sig, SIG_IGN);
//    exitProducers = 1;
    for (int i = 0; i < numProdThreads; i++){
        pthread_join(producerThreads[i],NULL );
        printf("Producer %d, Exiting.\n", i+1);
//        pthread_exit(producerThreads[i]);
    }
//    exitConsumers = 1;
    for (int i = 0; i < numConsThreads; i++){
        pthread_join(consumerThreads[i],NULL );
        printf("Consumer %d, Exiting.\n", (i+1));
//        pthread_exit(consumerThreads[i]);
    }
    return 0;
}

void initItemBuffer(){
    for(int i =0; i < BUFFER_SIZE; i++){
        itemBuffer[i] = -1;
    }
}

void printItemBuffer(){
    for(int i = 0; i < BUFFER_SIZE; i++){
        printf("Itembuffer %d: value: %d", i, itemBuffer[i]);
    }
}
void initConsumerThreads(int numThreads){
    consumerThreads = malloc(sizeof(pthread_t)*numThreads);
    for (int i = 0; i < numThreads; i++){
        if(pthread_create(&consumerThreads[i],NULL, (void*)Consumer, (int*)i)!=0)
            fprintf(stderr, "pthread failed to create errno: %d consumerID: %d",errno, i);
        else
            printf("Init Consumer %d\n", i+1);
    }
}

void initMutexAndCondVars() {
    pthread_mutex_init(&buff_mutex, NULL);
    pthread_cond_init(&empty, NULL);
    pthread_cond_init(&full, NULL);
    exitProducers = 0;
    exitConsumers = 0;
}

void initProducerThreads(int numThreads){
    producerThreads = (pthread_t *)malloc(sizeof(pthread_t)*numThreads);
    for (int i = 0; i < numThreads; i++){
        if(pthread_create(&producerThreads[i],NULL, (void*)Producer, (int *)i)!=0)
            fprintf(stderr, "pthread failed to create errno: %d producerID: %d",errno, &i);
        else
            printf("Init Producer %d\n", i+1);
    }
}

int InsertItem(BufferItem item){
    // insert item into buffer
    printf("InsertItem\n");
    int i = 0;
    for(i; i < BUFFER_SIZE; i++){
        if (itemBuffer[i]== -1)
            break;
    }
    if (i == BUFFER_SIZE)
        return -1;// return -1 indicating an error condition
    else{
        printf("Adding %d to buffer\n", item);
        itemBuffer[i] = item;
        return 0;// return 0 if successful, otherwise
    }
}

void Producer(int prodId) {
    int randSleep = -1;
    int id = prodId;
    while(1) {
        if (exitProducers == 1)
            return;
        randSleep = rand()%runTime;

        BufferItem item = rand();

        while (pthread_mutex_lock(&buff_mutex) != 0)
            fprintf(stderr, "ERROR on pthread_mutex_lock. producerID: %d, errno:%d ", id, errno);
//        int result = InsertItem(item);
//        if (result == -1)
//            pthread_cond_signal(&full);
//        while(InsertItem(item) ==-1){
        if(InsertItem(item) == -1){
            printf("Full Buffer\n");
//            signal that the itemBuffer is
            pthread_cond_broadcast(&full);
//            wait for a signal that the buffer is consumed
//            pthread_cond_wait(&empty, &buff_mutex);
        }else{
            fprintf(stdout, "Producer %d Producing\n", id);
        }

        while (pthread_mutex_unlock(&buff_mutex) != 0)
            fprintf(stderr, "ERROR on pthread_mutex_unlock. producerID: %d, errno:%d ", id, errno);
        printf("producer %d sleeping: %d\n", id, randSleep);
        sleep(randSleep);
        fprintf(stdout, "Producer %d done sleeping: %d\n", prodId, randSleep);
        fflush(stdout);
    }
}

int RemoveItem(BufferItem *item){
    int i;
    for (i = 0; i < BUFFER_SIZE; i++){
        if(itemBuffer[i]!= -1) {
            // place it in item
            *item = itemBuffer[i];
            // remove an object from buffer
            itemBuffer[i] = -1;
            return 0;// return 0 if sucessful, otherwise
        }
    }
    return  -1;// return -1 indicating an error condition
}
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
#include <termio.h>
#include "buffer.h"

char* ShareMalloc(int);

BufferItem itemBuffer [BUFFER_SIZE];

pthread_t *producerThreads;
pthread_t *consumerThreads;
pthread_mutex_t buff_mutex;
pthread_cond_t empty;
pthread_cond_t full;
int exitThreads;
int runTime = 0;
int numProdThreads = 0;
int numConsThreads = 0;
struct termios termInfo, termSave;

void Consumer(int conId);
void EchoOff();
void EchoOn();
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
    EchoOff();
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
        printf("using default values\n Runtime: 15 seconds; Num Producer/Consumer Threads: 10\n");
        runTime = 15;
        numProdThreads = 10;
        numConsThreads = 10;
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
    while(exitThreads == 0) {
//        printf("main sleeping\n");
        sleep(runTime);
    }
//    printf("main DONE sleeping Raising Quit Alarm\n");
//        raise signal for threads to quit
    raise(SIGINT);
//    }

//    6. Exit



    Exit(1);
    free(producerThreads);
    free(consumerThreads);
    printf("Main Exiting\n");
    return 0;

}


void Consumer(int conId) {
    int id = conId + 1;
    BufferItem item = 0;
//    printf("Consumer %d!\n", conId);
//    int id = conId;
//    signal(SIGINT, ThreadExit(id));
    int randSleep;
    while(1) {
        randSleep = rand()%10;
        if (exitThreads == 1) {
//            pthread_mutex_unlock(&buff_mutex);
            return;
        }
        while (pthread_mutex_lock(&buff_mutex) != 0)
            fprintf(stderr, "ERROR on pthread_mutex_lock. consumerID: %d, errno:%d ", id, errno);
        if(RemoveItem(&item) == -1){
//            printf("Empty Buffer!\n");
            pthread_cond_broadcast(&empty);
        } else {
            printf("Consumer %d Consumed %d\n", id, item);
        }
        while (pthread_mutex_unlock(&buff_mutex) != 0)
            fprintf(stderr, "ERROR on pthread_mutex_unlock. consumerID: %d, errno:%d ", id, errno);
        sleep(randSleep);
//        fprintf(stdout, "Consumer %d done sleeping: %d\n", id, randSleep);
        fflush(stdout);
    }
}

void EchoOff(){
    char c = tcgetattr(0,&termInfo);

    if(c == -1 ){
        perror("tcgetattr\n");
    }
    if(termInfo.c_lflag & ECHO) {
        termInfo.c_lflag &= ~(ECHO | ECHOE | ECHOK | ECHONL);
        tcsetattr(STDOUT_FILENO, TCSANOW, &termInfo);
    }

}

//void EchoOn(){
//    if(!(termInfo.c_lflag & ECHO)) {
//        tcsetattr(STDOUT_FILENO, TCSANOW, &termSave);
//    }
//}

int Exit(int sig) {
//    printf("Exiting\n");
    for (int i = 0; i < numProdThreads; i++){
        pthread_join(producerThreads[i],NULL );
        printf("Producer %d, Exiting.\n", i+1);
    }
    for (int i = 0; i < numConsThreads; i++){
        pthread_join(consumerThreads[i],NULL );
        printf("Consumer %d, Exiting.\n", i+1);
    }
    return 0;
}

void initItemBuffer(){
    for(int i =0; i < BUFFER_SIZE; i++){
        itemBuffer[i] = -1;
    }
}

void initConsumerThreads(int numThreads){
    consumerThreads = malloc(sizeof(pthread_t)*numThreads);
    for (int i = 0; i < numThreads; i++){
        if(pthread_create(&consumerThreads[i],NULL, (void*)Consumer, (int*)i)!=0)
            fprintf(stderr, "pthread failed to create errno: %d consumerID: %d",errno, i+1);
//        else
//            printf("Init Consumer %d\n", i+1);
    }
}

void initMutexAndCondVars() {
    pthread_mutex_init(&buff_mutex, NULL);
    pthread_cond_init(&empty, NULL);
    pthread_cond_init(&full, NULL);
    exitThreads = 0;
}

void initProducerThreads(int numThreads){
    producerThreads = (pthread_t *)malloc(sizeof(pthread_t)*numThreads);
    for (int i = 0; i < numThreads; i++){
        if(pthread_create(&producerThreads[i],NULL, (void*)Producer, (int *)i)!=0)
            fprintf(stderr, "pthread failed to create errno: %d producerID: %d",errno, i+1);
//        else
//            printf("Init Producer %d\n", i+1);
    }
}

int InsertItem(BufferItem item){
    // insert item into buffer
//    printf("InsertItem\n");
    int i = 0;
    for(i; i < BUFFER_SIZE; i++){
        if (itemBuffer[i]== -1)
            break;
    }
    if (i == BUFFER_SIZE)
        return -1;// return -1 indicating an error condition
    else{
//        printf("Adding %d to buffer\n", item);
        itemBuffer[i] = item;
        return 0;// return 0 if successful, otherwise
    }
}

void Producer(int prodId) {
    int randSleep = -1;
    int id = prodId + 1;
    while(1) {
        if (exitThreads == 1) {
//            pthread_mutex_unlock(&buff_mutex);
            return;
        }
        randSleep = rand()%10;

        BufferItem item = rand();

        while (pthread_mutex_lock(&buff_mutex) != 0)
            fprintf(stderr, "ERROR on pthread_mutex_lock. producerID: %d, errno:%d ", id, errno);
        if(InsertItem(item) == -1){
//            printf("Full Buffer\n");
            pthread_cond_broadcast(&full);
        }
        else{
            fprintf(stdout, "Producer %d Producing %d\n", id, item);
        }

        while (pthread_mutex_unlock(&buff_mutex) != 0)
            fprintf(stderr, "ERROR on pthread_mutex_unlock. producerID: %d, errno:%d ", id, errno);
//        printf("producer %d sleeping: %d\n", id, randSleep);
        sleep(randSleep);
//        fprintf(stdout, "Producer %d done sleeping: %d\n", id, randSleep);
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

void ThreadExit(int sig){
    signal(SIGINT, ThreadExit);
    printf("SIGINT signal recieved Quitting Program NOW.\n");
    exitThreads = 1;
}

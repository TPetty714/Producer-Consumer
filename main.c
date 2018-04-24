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

pthread_mutex_t buff_mutex;
pthread_cond_t empty;
pthread_cond_t full;

char* ShareMalloc(int);


BufferItem itemBuffer [BUFFER_SIZE];

void *Consumer(void *param);
int InsertItem(BufferItem item);
void *Producer(void *param);
int RemoveItem(BufferItem *item);

pthread_t *producerThreads;
pthread_t *consumerThreads;

//void *Consumer(void *param){
//    BufferItem item;
//    while(true){
////        sleep for a random period of time
////        sleep();
//        if(RemoveItem(&item)){
//            fprintf(stderr,"remove item error");
//        }else{
//            printf("consumer consumed %d\n", item);
//        }
//    }
//}

int InsertItem(BufferItem item){
		// insert item into buffer
		// return 0 if successful, otherwise
//	printf("Adding %d to buffer\n", item);
	return 0;
		// return -1 indicating an error condition
}


int Exit(pthread_t *producers, int num_prod, pthread_t *consumers, int num_consum){
	for (int i = 0; i < num_prod; i++){
		pthread_join( producers[i],NULL );
		printf("Producer %d, done.\n", i+1);
	}
	for (int i = 0; i < num_consum; i++){
		pthread_join( consumers[i],NULL );
		printf("Consumer %d, done.\n", i+1);
	}
	return 0;
}

void *Producer(void* prodId) {
    int randSleep = rand();
	while(1) {
		while (pthread_mutex_lock(&buff_mutex) != 0)
			fprintf(stderr, "ERROR on pthread_mutex_lock. producerID: %d, errno:%d ", prodId, errno);
		InsertItem(prodId);
		while (pthread_mutex_unlock(&buff_mutex) != 0)
			fprintf(stderr, "ERROR on pthread_mutex_unlock. producerID: %d, errno:%d ", prodId, errno);
	}
}

void *Consumer(void *conId) {
	while(1) {
		while (pthread_mutex_lock(&buff_mutex) != 0)
			fprintf(stderr, "ERROR on pthread_mutex_lock. consumerID: %d, errno:%d ", conId + 1, errno);
		RemoveItem(conId);
		while (pthread_mutex_unlock(&buff_mutex) != 0)
			fprintf(stderr, "ERROR on pthread_mutex_unlock. consumerID: %d, errno:%d ", conId + 1, errno);
	}
}

int main(int argc,char** argv)
{
//    1. get command line arguments argv[1], argv[2], argv[3]
    char* c = argv[1];
    int sleepTime = 0;
    int numProdThreads = 0;
    int numConsThreads = 0;

    sscanf(argv[1],"%d", &sleepTime);
    sscanf(argv[2], "%d", &numProdThreads);
    sscanf(argv[3], "%d", &numConsThreads);

    printf("sleeptime: %d \n", sleepTime);
    printf("producerSize: %d\n", numProdThreads);
    printf("producerSize: %d\n", numConsThreads);

//    sleep time, number producers, number consumers

//    2. initialize buffer

//    2a. initialize mutex for buffer
	pthread_mutex_init(&buff_mutex, NULL);
//    3. create producer thread(s)
	initProducerThreads(numProdThreads);

//    4. create consumer thread(s)
	initConsumerThreads(numConsThreads);

//    5. sleep
    sleep(100);
//    6. Exit
	Exit(producerThreads, numProdThreads, consumerThreads, numConsThreads);
	return 0;
}

void initProducerThreads(int numThreads){
    producerThreads = malloc(sizeof(pthread_t)*numThreads);
    for (int i = 0; i < numThreads; i++){
        if(pthread_create(&producerThreads[i],NULL, Producer, &i)!=0)
            fprintf(stderr, "pthread failed to create errno: %d producerID: %d",errno, i);
        else
            printf("Producer %d Producing\n", i+1);
    }
}
void initConsumerThreads(int numThreads){
    consumerThreads = malloc(sizeof(pthread_t)*numThreads);
    for (int i = 0; i < numThreads; i++){
        if(pthread_create(&consumerThreads[i],NULL, Consumer, &i)!=0)
            fprintf(stderr, "pthread failed to create errno: %d consumerID: %d",errno, i);
        else
            printf("Consumer %d consuming\n", i+1);
    }
}

//void *Producer(void *param){
//    BufferItem item;
//    while(true){
////        sleep for a random period of time
////        sleep();
////        generate a random number
//        item = rand();
//        if(InsertItem(item)){
//            fprintf(stderr, "report error");
//        }else{
//            printf("producer produced %d\n", item);
//        }
//    }
//}


int RemoveItem(BufferItem *item){
    // remove an object from buffer
    // place it in item
    // return 0 if sucessful, otherwise
    // return -1 indicating an error condition
    return 0;
}


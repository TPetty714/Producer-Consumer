#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include <errno.h>
#include <math.h>
#include <time.h>
#include <pthread.h>
#include "buffer.h"

pthread_mutex_t buff_mutex;
char* ShareMalloc(int);


BufferItem buffer [BUFFER_SIZE];

int InsertItem(BufferItem item){
		// insert item into buffer
		// return 0 if successful, otherwise
	printf("Adding %d to buffer\n", item);
	return 0;
		// return -1 indicating an error condition
}

int RemoveItem(BufferItem *item){
		// remove an object from buffer
		// place it in item
		// return 0 if sucessful, otherwise
	printf("Removing %d from buffer\n", item);
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

void Producer(int prodId) {
	while(1) {
		while (pthread_mutex_lock(&buff_mutex) != 0)
			fprintf(stderr, "ERROR on pthread_mutex_lock. producerID: %d, errno:%d ", prodId, errno);
		InsertItem(prodId);
		while (pthread_mutex_unlock(&buff_mutex) != 0)
			fprintf(stderr, "ERROR on pthread_mutex_unlock. producerID: %d, errno:%d ", prodId, errno);
	}
}

void Consumer(int conId) {
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
	int num_prod = 10;
	int num_consum = 10;
//    2. initialize buffer

//    2a. initialize mutex for buffer
	pthread_mutex_init(&buff_mutex, NULL);
//    3. create producer thread(s)
	pthread_t *producer_array = malloc(sizeof(pthread_t)*num_prod);
	for (int i = 0; i < num_prod; i++)
		if(pthread_create(&producer_array[i],NULL, (void*)Producer, (int*)i)!=0)
			fprintf(stderr, "pthread failed to create errno: %d producerID: %d",errno, i);
		else
			printf("Producer %d Producing\n", i+1);
//    4. create consumer thread(s)
	pthread_t *consumer_array = malloc(sizeof(pthread_t)*num_consum);
	for (int i = 0; i < num_consum; i++)
		if(pthread_create(&consumer_array[i],NULL, (void*)Consumer, (int*)i)!=0)
			fprintf(stderr, "pthread failed to create errno: %d consumerID: %d",errno, i);
		else
			printf("Consumer %d consuming\n", i+1);
	sleep(100);
//    5. sleep
//    6. Exit
	Exit(*producer_array, num_prod, *consumer_array, num_consum);
	return 0;
}

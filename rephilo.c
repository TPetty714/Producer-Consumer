//  Philosophers Problem with Threads
//  Author: Dennis Vargas
//  Course: cs360 @ Washington State University Vancouver
//  Description:    Runs a simulated model of the Philosophers problem
//                  where in 5 philosphers sit at table with 5 chopsticks
//                  one on either side of each philosopher. Each philospher may only
//                  eat when they have both chopsticks. Each philospher
//                  eats an amount of time then waits an amount of time.
//                  Each philospher will eat a maximum of 100 seconds and
//                  then they leave the table.

//#define _BSD_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include <errno.h>
#include <math.h>
#include <time.h>
#include <pthread.h>

#define NUM_PHILO 5
#define ENDING_EAT_TIME 100
#define MEAN_EAT 8
#define STDDEV_EAT 3
#define MEAN_WAIT 10
#define STDDEV_WAIT 4

//  Global Variables
int chopsticks[NUM_PHILO] = {-1,-1,-1,-1,-1};
pthread_mutex_t chop_mutex;

//  Function Prototypes
char* ShareMalloc(int);
void Philo(int);
int randomGaussian(int mean, int stddev);

int main() {
    pthread_mutex_init(&chop_mutex, NULL);
    pthread_t *thread_array = malloc(sizeof(pthread_t)*NUM_PHILO);

    for (int i = 0; i < NUM_PHILO; i++)
        if(pthread_create(&thread_array[i],NULL, (void*)Philo, (void*)i)!=0)
            fprintf(stderr, "pthread failed to create errno: %d philoID: %d",errno, i);
        else
            printf("Philospher %d sits at the table.\n", i+1);

    for (int i = 0; i < NUM_PHILO; i++){
        pthread_join(thread_array[i],NULL);
        printf("philosopher %d, done.\n", i+1);
    }

    return 0;
}// main()


void Philo(int philoId){

    unsigned seed_state = (unsigned)philoId+1;
    int eat_total = 90;// stores total eaten by philosopher
    unsigned eat_amount, wait_amount = 0;//  holds the random amount the philosopher will eat
    int chopstickA, chopstickB;//  pointers that hold location of chopsticks in shared memory

    while(eat_total <= ENDING_EAT_TIME){
        while(pthread_mutex_lock(&chop_mutex)!=0)
            fprintf(stderr, "ERROR on pthread_mutex_lock. philoID: %d, errno:%d ", philoId+1,errno);
//      set chopstickA to the location in shared memory designated to this philospher
        chopstickA = chopsticks[philoId];
//      if philospher the last in numeric order
        if (philoId == NUM_PHILO-1){
            chopstickB = chopsticks[0];//   chopstickB is the first index iff philoID ==NUM_PHILO
        }else
            chopstickB = chopsticks[philoId+1];// chopstickB is chopstick at shared memory offset by philoID

//      if chopstickA and B are available
        if ((chopstickA == -1)&&(chopstickB==-1)){
            chopstickA = chopsticks [philoId] = philoId;// set sharedMem value and chopstickA to philoID
            if(philoId == NUM_PHILO-1)
                chopstickB = chopsticks[0] = philoId;//   set sharedMem and chopstickB to philoID
            else
                chopstickB = chopsticks[philoId+1] = philoId;
        }
        //  enable the semaphore for next philosopher to pickup chopsticks
        while(pthread_mutex_unlock(&chop_mutex)!=0)
            fprintf(stderr, "ERROR on pthread_mutex_unlock. philoID: %d, errno:%d ", philoId+1,errno);


//      If the philosopher has chopsticks
        if((chopstickA == philoId)&&(chopstickB==philoId)){
            eat_amount = 90;
            while((eat_amount = abs(randomGaussian_r(MEAN_EAT, STDDEV_EAT, &seed_state)))==0);
            eat_total+= eat_amount;//   sum current total and eat amount and store in eat amount
            printf("Philosopher %d Eating for %d\n", philoId+1, eat_amount);
            sleep(eat_amount);
            printf("Philosopher %d, Eaten %d/%d\n", philoId+1,eat_total,ENDING_EAT_TIME);
            while(pthread_mutex_lock(&chop_mutex)!=0)
                fprintf(stderr, "ERROR on pthread_mutex_lock. philoID: %d, errno:%d ", philoId+1,errno);
            chopstickA = chopsticks[philoId] = -1;
            if(philoId == NUM_PHILO-1)
                chopstickB = chopsticks[0] = -1;
            else
                chopstickB = chopsticks[philoId+1] = -1;

            while(pthread_mutex_unlock(&chop_mutex)!=0)
                fprintf(stderr, "ERROR on pthread_mutex_unlock. philoID: %d, errno:%d ", philoId+1,errno);
            wait_amount = 0;
            while((wait_amount = randomGaussian_r(MEAN_WAIT, STDDEV_EAT, &seed_state))==0);
            printf("Philosopher %d, waiting for %d seconds\n", philoId+1, wait_amount);
            sleep(wait_amount);
           }

    }
    printf("Philosopher %d has left the table.\n",philoId+1);

}// Philo


int randomGaussian_r(int mean, int stddev, unsigned int* state) {
    double mu = 0.5 + (double) mean;
    double sigma = fabs((double) stddev);
    double f1 = sqrt(-2.0 * log((double) rand_r(state) / (double) RAND_MAX));
    double f2 = 2.0 * 3.14159265359 * (double) rand_r(state) / (double) RAND_MAX;
    if (rand_r(state) & (1 << 5))
        return (int) floor(mu + sigma * cos(f2) * f1);
    else
        return (int) floor(mu + sigma * sin(f2) * f1);
}// randomGaussian
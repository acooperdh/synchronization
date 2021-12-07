#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdbool.h>
#include <unistd.h>
#include "asn5.h"

#define BUFFER_SIZE 5

/* Notes: 
1. producers and consumers running as different threads, move items to
and from the buffer 
2. like in Chapter 7, we are using empty, full (both semaphores) and 
mutex. 
    Therefore 2 unnamed semaphores & 1 mutex lock instead of a binary 
    semaphores 





*/
// global variables
typedef int buffer_item;

// mutex
pthread_mutex_t mutex;

// semaphore 
sem_t full;
sem_t empty;

int in = 0;
int out = 0;

// init buffer
buffer_item buffer[BUFFER_SIZE];

// inserts an item into the array at the first possible spot 
// when an item is inserted, in is moved one forward so that the next item knows where to go 
// if in == buffer size no more items can be inserted 

void set_counters(){
    if (in < BUFFER_SIZE && out < BUFFER_SIZE) return;
    if (in == BUFFER_SIZE){
        for(int i = 0; i < BUFFER_SIZE; i++){
            if (buffer[i] == -1){
                in = i;
                break;
            }
        }
    } 
}

void insert_item(int num){
    printf("inserting item %d\n", num);
    for(int i = 0; i < BUFFER_SIZE; i++){
        if (buffer[i] == -1){
            buffer[i] = num;
            printf("%d inserted at %d\n", num, i);
            return;
        }
    }
    printf("was unable to insert %d\n",num);
    return;
}

// removes item from the array starting at 0 and working its way to the end 
// when item removed, out should be changed to the index of the earliest item
void remove_item(){
    printf("removing the earliest item in the buffer\n");
    for(int i = 0; i < BUFFER_SIZE; i++){
        if (buffer[i] != -1){
            int num = buffer[i];
            buffer[i] = -1;
            printf("%d removed at %d\n", num, i);
            return;
        }
    }
    printf("was unable to remove because the buffer is empty\n");
    return;
}

/*
structure of process 
while(true){
    // pre critical section code 
    wait() make request to enter the semaphore 
    // critical section code 
    signal() release the semaphore and continue on your day 
    // remainder section 
}
*/
void* producer(){
    buffer_item item;
    sem_wait(&empty);
    pthread_mutex_lock(&mutex);
    // sleep for a random period of time: 0 - 4 seconds 
    int time = rand() % 4;
    sleep(time);
    // generate a random number 
    item = rand();
    // insert an item 
    insert_item(item);
    
    pthread_mutex_unlock(&mutex);
    sem_wait(&full);
}

void* consumer(void* param){
        // request semaphore and mutex
        while(true){
            sem_wait(&full);
        pthread_mutex_lock(&mutex);
        // sleep for random period of time 
        sleep(1);
        // remove item
        remove_item();
        // release mutex & semaphore
        pthread_mutex_unlock(&mutex);
        sem_wait(&empty);
    }
        
}

int main(int argc, char *argv[]){
    // getting command line arguements 
    int sleep_time = atoi(argv[0]);
    int num_producers = atoi(argv[1]);
    int num_consumers = atoi(argv[2]);
    int in; // indicate the next free slot in the buffer 
    int out; // indicate the earliest item in the buffer
    // init mutex 
    pthread_mutex_init(&mutex, NULL);

    // init semaphore
    sem_init(&full, 1, 0);
    sem_init(&empty, 1, BUFFER_SIZE);
    // init buffer
    for(int i = 0; i < BUFFER_SIZE; i++){
        buffer[i] = -1;
    }

    // creating producer threads 
    pthread_t producers[num_producers];
    for(int i = 0; i < num_producers; i++){
        pthread_t t;
        producers[i] = pthread_create(&t, NULL, producer, NULL);
        int temp = rand() % 4;
    }
    //creating consumer threads 
    pthread_t consumers[num_consumers];
    for(int i = 0; i < num_consumers; i++){
        pthread_t t;
        consumers[i] = pthread_create(&t, NULL, consumer, NULL);
        int temp = rand() % 4;
    }

    int total_threads = num_consumers + num_producers;
    for(int i = 0; i < total_threads; i++){
        if (i < num_consumers){
            pthread_join(consumers[i], NULL);
        }
        if (i < num_producers){
            pthread_join(producers[i], NULL);
        }
        if (i >= num_producers && i >= num_consumers){
            break;
        }
    }

    /*
    1. get command line args argv[1], argv[2], argv[3]
    2. init semaphores and mutex lock
    3. init buffer
    4. create producer thread(s)
    5. create consumer thread(s)
    6. sleep and thereafter terminate the C program
    */

    pthread_mutex_destroy(&mutex);
    sem_destroy(&full);
    sem_destroy(&empty);
}
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdbool.h>
#include <unistd.h>
#include "asn5.h"

#define BUFFER_SIZE 5

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
int buffer[BUFFER_SIZE];


int update_position(int pos){
    pos+=1;
    return (pos % BUFFER_SIZE);
}
// inserts an item into the array at the first possible spot 
// when an item is inserted, in is moved one forward so that the next item knows where to go 
// if in == buffer size no more items can be inserted 

void insert_item(int num, int index){
    buffer[in] = num;
    printf("Producer %d put %d at %d\n", index, num, in);
    update_position(in);
}
// removes item from the array starting at 0 and working its way to the end 
// when item removed, out should be changed to the index of the earliest item
void remove_item(void* index_ptr){
    int element = buffer[out];
    buffer[out] = -1;
    printf("Consumer %d remove %d at %d\n", *((int *)index_ptr), element, out);
    update_position(out);
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
void* producer(void *index){
    int ind = *((int*)index);
    while(1){
        buffer_item item;
        sem_wait(&empty);
        pthread_mutex_lock(&mutex);
        // sleep for a random period of time: 0 - 4 seconds 
        sleep(rand()%4);
        // generate a random number 
        item = rand();
        // insert an item 
        insert_item(item, ind);
        pthread_mutex_unlock(&mutex);
        sem_wait(&full);
    }
}
// consumer function
void* consumer(void* index){
    // request semaphore and mutex
    while(1){
        sem_wait(&full);
        pthread_mutex_lock(&mutex);
        // sleep for random period of time 
        sleep(rand() % 4);
        // remove item
        remove_item(index);
        // release mutex & semaphore
        pthread_mutex_unlock(&mutex);
        sem_wait(&empty);
        break;
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
    printf("making it here");
    // init semaphore
    sem_init(&full, 0, 0);
    sem_init(&empty, 0, BUFFER_SIZE);
    // init buffer
    for(int i = 0; i < BUFFER_SIZE; i++){
        buffer[i] = -1;
    }

    // creating producer threads 
    pthread_t producers[num_producers];
    for(int i = 0; i < num_producers; i++){
        pthread_create(&producers[i], NULL, producer, (void*)&i);
        sleep(2);
    }
    //creating consumer threads 
    pthread_t consumers[num_consumers];
    for(int i = 0; i < num_consumers; i++){
        pthread_create(&consumers[i], NULL, consumer, NULL);
        sleep(2);
    }
    for(int i = 0; i < num_producers; i++){
        pthread_join(producers[i], NULL);
    }
    for(int i = 0; i < num_consumers; i++){
        pthread_join(consumers[i], NULL);
    }

    pthread_mutex_destroy(&mutex);
    sem_destroy(&full);
    sem_destroy(&empty);
}
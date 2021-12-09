// Drew Cooper B00811386 CSCI 3120 Assignment 5 - Synchronization

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

#define BUFFER_SIZE 5

typedef int buffer_item;

pthread_mutex_t mutex;

sem_t full;
sem_t empty;

int in = 0;
int out = 0;

int buffer[BUFFER_SIZE];

int numProducers = 0;
int numConsumers = 0;
int sleepTime = 0;

// used to store the index of a thread
typedef struct thread_param{
	int index;
} ThreadParam;

int update_position(int pos){
	int temp = pos += 1;
	return (pos % BUFFER_SIZE);
}

void insert_item(int index, buffer_item item){
	buffer[in] = item;
	printf("Producer %d inserted item %d into buffer[%d]\n", index, item, in);
	in = update_position(in);
	return;
}

void remove_item(int index){
	buffer_item item = buffer[out];
	printf("Consumer %d removed item %d from buffer[%d]\n", index, item, out);
	buffer[out] = -1;
	out = update_position(out);
	return;
}

void* producer(void* index){
	buffer_item item;
	while(1){
		// sleep
		sleep(rand() % 4);
		ThreadParam* temp_ptr = (ThreadParam*)index;
		int ind = temp_ptr->index;
		// generate random number
		item = rand();
		//request access
		sem_wait(&empty);
		pthread_mutex_lock(&mutex);
		// critical section
		insert_item(ind, item);
		// release access
		pthread_mutex_unlock(&mutex);
		sem_post(&full);
	}
}

void* consumer(void* index){
	while(1){
		// sleep
		sleep(rand() % 4);
		ThreadParam* temp_ptr = (ThreadParam*)index;
		int ind = temp_ptr->index;
		// request access
		sem_wait(&full);
		pthread_mutex_lock(&mutex);
		// critical section
		remove_item(ind);
		//release access
		pthread_mutex_unlock(&mutex);
		sem_post(&empty);
	}
}

int main(int argc, char *argv[]){
	pthread_t producers[numProducers];
	pthread_t consumers[numConsumers];

	pthread_mutex_init(&mutex, NULL);
	sem_init(&full, 0, 0);
	sem_init(&empty, 0, BUFFER_SIZE);
	srand(time(NULL));
	if (argc != 4) return 1;
	sleepTime = atoi(argv[1]);
	numProducers = atoi(argv[2]);
	numConsumers = atoi(argv[3]);

	// creating producer threads
	for(int i = 0; i < numProducers; i++){
		ThreadParam* p = malloc(sizeof(int)*2);
		p->index = i;
		pthread_create(&producers[i], NULL, producer, p);
	}

	// creating consumer threads
	for(int x = 0; x < numConsumers; x++){
		ThreadParam* p = malloc(sizeof(int)*2);
		p->index = x;
		pthread_create(&consumers[x], NULL, consumer, p);
	}

	// sleep to allow the threads to run 
	sleep(sleepTime);

	// destroying mutex and semaphores before terminating the program 
	pthread_mutex_destroy(&mutex);
	sem_destroy(&empty);
	sem_destroy(&full);
	return 0;
}

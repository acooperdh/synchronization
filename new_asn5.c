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

typedef struct voidptr{
	int index;
} ThreadParam;

int update_position(int pos){
	int temp = pos += 1;
	return (pos % BUFFER_SIZE);
}

void* producer(void* index){
	buffer_item item;
	while(true){
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
		buffer[in] = item;
		printf("Producer %d inserted item %d into buffer[%d]\n", ind, item, in);
		in = (in+1) % BUFFER_SIZE;
		// release access
		pthread_mutex_unlock(&mutex);
		sem_post(&full);
		// remainder code
	}
}

void* consumer(void* index){
	while(true){
		// sleep
		sleep(rand() % 4);
		ThreadParam* temp_ptr = (ThreadParam*)index;
		int ind = temp_ptr->index;

		// request access
		sem_wait(&full);
		pthread_mutex_lock(&mutex);
		// critical section
		buffer_item item = buffer[out];
		buffer[out] = -1;
		printf("Consumer %d removed item %d from buffer[%d]\n", ind, item, out);
		buffer[out] = -1;
		out = (out + 1) % BUFFER_SIZE;
		//release access
		pthread_mutex_unlock(&mutex);
		sem_post(&empty);
		// remainder code
	}
}

int main(int argc, char *argv[]){
	pthread_t producers[numProducers];
	pthread_t consumers[numConsumers];
	sem_init(&full, 0, 0);
	sem_init(&empty, 0, BUFFER_SIZE);

	if (argc != 4) return 1;
	int sleepTime = atoi(argv[1]);
	numProducers = atoi(argv[2]);
	numConsumers = atoi(argv[3]);
	for(int i = 0; i < numProducers; i++){
		ThreadParam* p = malloc(sizeof(int)*2);
		p->index = i;
		pthread_create(&producers[i], NULL, producer, p);
	}
	for(int i = 0; i < numConsumers; i++){
		ThreadParam* p = malloc(sizeof(int)*2);
		p->index = i;
		pthread_create(&consumers[i], NULL, consumer, p);
	}

	// pthread_mutex_destroy(&mutex);
	// sem_destroy(&empty);
	// sem_destroy(&full);
	sleep(sleepTime);
	return 0;
}

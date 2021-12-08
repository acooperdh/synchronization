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

void* producer(void* index){
	buffer_item item;
	for(int i = 0; i < numProducers; i++){
		ThreadParam* temp_ptr = (ThreadParam*)index;
		int ind = temp_ptr->index;
		item = rand();
		sem_wait(&empty);
		pthread_mutex_lock(&mutex);
		buffer[in] = item;
		printf("Producer %d inserted item %d into buffer[%d]\n", ind, item, in);
		in = (in+1)%BUFFER_SIZE;
		pthread_mutex_unlock(&mutex);
		sem_post(&full);
	}
}

void* consumer(void* index){
	for(int i = 0; i < numConsumers; i++){
		ThreadParam* temp_ptr = (ThreadParam*)index;
		int ind = temp_ptr->index;
		sem_wait(&full);
		pthread_mutex_lock(&mutex);
		buffer_item item = buffer[out];
		printf("Consumer %d removed item %d from buffer[%d]\n", ind, item, out);
		out = (out+1)%BUFFER_SIZE;
		pthread_mutex_unlock(&mutex);
		sem_post(&empty);
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

	for(int i = 0; i < numProducers; i++){
		pthread_join(producers[i], NULL);
	}
	for(int i = 0; i < numConsumers; i++){
		pthread_join(consumers[i], NULL);
	}
	pthread_mutex_destroy(&mutex);
	sem_destroy(&empty);
	sem_destroy(&full);
	return 0;
}

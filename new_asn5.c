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

int numProducers = 3;
int numConsumers = 3;
void* producer(void* index){
	buffer_item item;
	for(int i = 0; i < numProducers; i++){
		item = rand();
		sem_wait(&empty);
		pthread_mutex_lock(&mutex);
		buffer[in] = item;
		printf("producer did a thing %d\n", item);
		in = (in+1)%BUFFER_SIZE;
		pthread_mutex_unlock(&mutex);
		sem_post(&full);
	}
}

void* consumer(void* index){
	for(int i = 0; i < numConsumers; i++){
		sem_wait(&full);
		pthread_mutex_lock(&mutex);
		buffer_item item = buffer[out];
		printf("consumer did a thing %d\n", item);
		out = (out+1)%BUFFER_SIZE;
		pthread_mutex_unlock(&mutex);
		sem_post(&empty);
	}
}

int main(){
	pthread_t producers[numProducers];
	pthread_t consumers[numConsumers];
	sem_init(&full, 0, 0);
	sem_init(&empty, 0, BUFFER_SIZE);
	sleep(rand() % 10);
	for(int i = 0; i < numProducers; i++){
		pthread_create(&producers[i], NULL, producer, (void *)&i);
	}
	for(int i = 0; i < numConsumers; i++){
		pthread_create(&consumers[i], NULL, consumer, (void *)&i);
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

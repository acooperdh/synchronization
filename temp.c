#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

sem_t full;
sem_t empty;

sem_t mutex;
  
void* thread(void* arg)
{
    //wait
    sem_wait(&mutex);
    printf("\nEntered..\n");
  
    //critical section
    sleep(4);
      
    //signal
    printf("\nJust Exiting...\n");
    sem_post(&mutex);
}
  
  
int main()
{
    sem_init(&mutex, 0, 1);
    pthread_t pool[2];
    pthread_t t1,t2;
    pthread_create(&pool[0],NULL,thread,NULL);
    sleep(2);
    pthread_create(&pool[1],NULL,thread,NULL);
    pthread_join(pool[0],NULL);
    pthread_join(pool[1],NULL);
    sem_destroy(&mutex);
    return 0;
}
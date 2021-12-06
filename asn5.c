#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdbool.h>
#include "asn5.h"

typedef int buffer_item;

void* producer(void* param){
    buffer_item item;
    while (true){
        // sleep for a random period of time: 0 - 4 seconds 
        sleep();
        // generate a random number 
        item = rand()
        // insert an item 
        insert_item();
    }
}

void* consumer(void* param){
    while(true){
        // sleep for random period of time 
        sleep()
        // remove item
        remove_item()
    }
}
int main(int argc, char *argv[]){
    /*
    1. get command line args argv[1], argv[2], argv[3]
    2. init semaphores and mutex lock
    3. init buffer
    4. create producer thread(s)
    5. create consumer thread(s)
    6. sleep and thereafter terminate the C program
    */

}
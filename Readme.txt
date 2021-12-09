Drew Cooper B00811386 CSCI 3120 Assignment 5 

Synchronization

This program implements a mutex and 2 unnamed semaphores in order to create a solution 
the bounded buffer problem, using a circular array

In order to compile the program, there is a Makefile that allows the user to compile 
without having to type out the gcc instructions each time. 
The command to compile is 
    make
or you can use 
    make main

if the Makefile does not work for some reason you can compile the program like such 
    gcc -o BoundedBuffer bounded_buffer.c -lpthread -lrt

in order to run the program, use 
    ./BoundedBuffer X Y Z
where X, Y, Z are all positive intergers 
X represents the amount of time you wish for the main function / thread to sleep while the producer 
and consumer threads are running 
Y represents the number of producer threads you wish to create 
Z represents the number of consumer threads you wish to create 

Ex. ./BoundedBuffer 10 3 2 would result in the main function / thread to sleep for 10 
seconds while the producer and consumer threads run. There would be 3 producer threads created 
and 2 consumer threads created. 

in order to delete the executable, simple run 
    make clean 
and this will delete BoundedBuffer
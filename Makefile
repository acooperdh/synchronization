main: bounded_buffer.c
	gcc -o BoundedBuffer bounded_buffer.c -lpthread -lrt
clean: 
	rm BoundedBuffer
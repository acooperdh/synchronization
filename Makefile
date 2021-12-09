main: new_asn5.c asn5.h
	gcc -o pls_work new_asn5.c -lpthread -lrt
clean: 
	rm pls_work
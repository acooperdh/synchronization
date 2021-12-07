main: asn5.c asn5.h
	gcc -o asn5 asn5.c -lpthread -lrt
clean: 
	rm asn5
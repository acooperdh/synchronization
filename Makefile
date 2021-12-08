main: new_asn5.c
	gcc -o pls_work new_asn5.c -lpthread -lrt
	./pls_work 2 1 5
clean: 
	rm new_asn5
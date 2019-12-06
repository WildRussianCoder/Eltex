all: sender.out listener.out server.out client1.out client2.out

sender.out: sender.c
	gcc -o sender.out sender.c -lrt -pthread

listener.out: listener.c
	gcc -o listener.out listener.c -lrt -pthread

server.out: server.c
	gcc -o server.out server.c -lrt 

client1.out: client1.c 
	gcc client1.c -o client1.out 

client2.out: client2.c 
	gcc client2.c -o client2.out 

clean:
	rm -f *.out 
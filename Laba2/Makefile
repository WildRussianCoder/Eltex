a.out: main.o
	gcc main.o
main.o: main.c
	gcc -c main.c

test: a.out
	./a.out < test.txt

clean:
	rm -rf a.out main.o
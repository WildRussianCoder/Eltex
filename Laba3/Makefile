a.out: main.c
	gcc main.c

test: a.out test.txt
	./a.out < test.txt

clean:
	rm -rf a.out
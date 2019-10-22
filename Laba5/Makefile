dynamic: mylib.o main.c
	gcc main.c -DDYNAMIC -ldl -o l5
	gcc -shared mylib.o -o libmylib.so

static: mylib.o main.c
	ar rc libmylib.a mylib.o
	ranlib libmylib.a 
	gcc main.c -DSTATIC -lmylib -L. -o l5

dynostatic: mylib.o main.c
	gcc -shared mylib.o -o libmylib.so
	gcc main.c -L. -DDYNOSTATIC -lmylib -o l5

mylib.o: mylib.c
	gcc -c mylib.c 
	
clean: 
	rm -rf mylib.o libmylib.so l5 libmylib.a


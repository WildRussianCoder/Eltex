l5: main.c
	gcc main.c -o l5  -ldl

lib: mylib.o
	gcc -shared mylib.o -o mylib.so
	sudo install mylib.so /usr/lib

mylib.o: mylib.c
	gcc -c mylib.c 
	
clean: 
	rm -rf mylib.o mylib.so l5 

remove:
	sudo rm -f /usr/lib/mylib.so
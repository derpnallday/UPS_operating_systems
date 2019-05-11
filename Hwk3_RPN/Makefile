all: main.c stack.o rpn.o
	gcc -Wall -o rpn main.c stack.o rpn.o 

rpn.o: rpn.h rpn.c
	gcc -Wall -c rpn.c 

stack.o: stack.h stack.c
	gcc -Wall -c stack.c 

clean:
	rm -f rpn *.o

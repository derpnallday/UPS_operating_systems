all: input.o queue.o instruction.o vmm.o main.c
	gcc -Wall -o vmm main.c input.o queue.o instruction.o vmm.o -lm

input.o: vmm.h input.c
	gcc -Wall -c input.c 

queue.o: vmm.h queue.c
	gcc -Wall -c queue.c 

instruction.o: vmm.h instruction.c
	gcc -Wall -c instruction.c 

vmm.o: vmm.h vmm.c
	gcc -Wall -c vmm.c 

clean:
	rm -f vmm *.o
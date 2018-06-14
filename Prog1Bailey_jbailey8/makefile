all: prog1sorter prog1generator

prog1sort: prog1sorter.o timer.o
	gcc  timer.o prog1sorter.o -o prog1sorter

timer.o: timer.c
	gcc -c timer.c

prog1sort.o: prog1sorter.c
	gcc -c prog1sorter.c

prog1generator: prog1generator.o
	gcc prog1generator.o -o prog1generator

prog1generator.o: prog1generator.c
	gcc -c prog1generator.c

clean:
	rm -f *.o prog1sorter prog1generator





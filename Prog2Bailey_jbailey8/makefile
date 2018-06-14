all: prog2tree

prog2tree: prog2tree.o
	gcc  prog2tree.o -o prog2tree

prog2tree.o: prog2tree.c
	gcc -c prog2tree.c

clean:
	rm -f *.o prog2tree


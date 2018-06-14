all: prog3ipc

prog3ipc: prog3ipc.o
	gcc -D_SVID_SOURCE -D_GNU_SOURCE -lrt -g prog3ipc.o -o prog3ipc

prog3ipc.o: prog3ipc.c
	gcc -c prog3ipc.c

clean:
	rm -f *.o prog3ipc

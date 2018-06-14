#Josiah Bailey, Alexander Lagas, Daniel Mohunlal
all: ssfs ssfs_mkdsk

ssfs: Common.cpp ssfs.cpp diskOp.cpp diskController.cpp Inode.cpp
	g++ -g -std=c++11 -pthread diskOp.cpp Common.cpp diskController.cpp Inode.cpp ssfs.cpp -o ssfs

ssfs_mkdsk: ssfs_mkdsk.c
	gcc -g ssfs_mkdsk.c -o ssfs_mkdsk

clean:
	rm -f *~ *.o ssfs ssfs_mkdsk

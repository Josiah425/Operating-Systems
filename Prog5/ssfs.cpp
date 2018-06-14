#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <iostream>
#include "diskOp.h"
#include "diskController.h"
#include "Common.h"

int main(int argc, char** argv){
	//Ensures that the program has the correct number of arguments
	if((argc < 3) || (argc > 6)){
		fprintf(stderr, "This program accepts arguments in the following format:\nssfs <disk-name> <operations-File>\nThere may be one through four <operations-File> arguments.\nSince arguments are not properly formatted, ");
		exit(0);
	}

	//Input file array
	FILE* inFiles[argc - 2];

	//Checks input files to ensure they have loaded correctly
	for(int i = 2; i < argc; i++){
		inFiles[i-2] = fopen(argv[i], "r");
		if(!inFiles[i-2]){
			fprintf(stderr, "Error: one or more files could not be loaded. Exiting.\n");
			exit(1);
		}
		fclose(inFiles[i-2]);
	}

	FILE* disk = fopen(argv[1], "rb+");
	readSuperBlock(disk);
	freeList = (bool *) malloc(sizeof(bool) * sb1.numBlocks);
	for(int i = 0; i < 257; i++){
		freeList[i] = 1;
	}
	readFreeBlockList(disk);
	for(int i = 0; i < 256; i++) Bmap[i].data = (char *) malloc(sizeof(char) * sb1.blockSize);
	readInodeMap(disk);
	LIST();

	//DC pthread
	pthread_t DC;

	//pthread DC thread
	if(pthread_create(&DC, NULL, diskController, (void *) argv[1])){
		fprintf(stderr, "Error: DC Thread creation failure. Program exiting.\n");
	}

	//DO pthreads
	pthread_t DO1, DO2, DO3, DO4;

	//DO pthread creation
	if(argc > 2) {
		if(pthread_create(&DO1, NULL, DiskOp, (void *) argv[2])){
			fprintf(stderr, "Error creating threads. Exiting...\n");
			exit(0);
		}
	}
	if(argc > 3){
		if(pthread_create(&DO2, NULL, DiskOp, (void *) argv[3])){
			fprintf(stderr, "Error creating threads. Exiting...\n");
			exit(0);
		}
	}
	if(argc > 4){
		if(pthread_create(&DO3, NULL, DiskOp, (void *) argv[4])){
			fprintf(stderr, "Error creating threads. Exiting...\n");
			exit(0);
		}
	}
	if(argc > 5){
		if(pthread_create(&DO4, NULL, DiskOp, (void *) argv[5])){
			fprintf(stderr, "Error creating threads. Exiting...\n");
			exit(0);
		}
	}

	//Clean up on completion
	pthread_join(DC, NULL);
	pthread_join(DO1, NULL);
	pthread_join(DO2, NULL);
	pthread_join(DO3, NULL);
	pthread_join(DO4, NULL);

	LIST();

	fclose(disk);

	return 0;
}

#ifndef COMMON_H
#define COMMON_H
#include <string>
#include <pthread.h>
#include "Inode.h"

typedef struct SB{
	int numBlocks;
	int blockSize;
	char * name;
} SB;

typedef struct Block{
	int blockSize;
	int blockNumber;
	//Needs 2 be Dynamic but gets messed up
	char * data;
} Block;

typedef struct InodeBlock{
	char name[32];
	int fileSize;
	int ptrs[14];
	bool InUse;
} InodeBlock;

extern Inode map[256];
extern Block Bmap[256];
extern pthread_mutex_t mutex;
extern pthread_mutex_t command;
extern pthread_cond_t empty;
extern pthread_cond_t filled;
const int MAX = 100;
extern std::string buffer[100];
extern int start;
extern int tail;

extern SB sb1;

extern bool * freeList;



#endif

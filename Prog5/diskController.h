#ifndef DISKCONTROLLER_H
#define DISKCONTROLLER_H

#include "Common.h"
#include "Inode.h"
#include <stdio.h>
#include <stdlib.h>
#include <string>

void readSuperBlock(FILE * disk);
void readInodeMap(FILE * disk);
void readFreeBlockList(FILE *disk);
void writeBlock(Block b, FILE * disk);
void writeInodeMap(FILE *disk);
void writeInodeBlock(InodeBlock b, int inodeNumber, FILE * disk);
void writeFreeBlockList(FILE *disk);
void consumeBuffer(FILE * disk);
int UploadNewFile(FILE * disk, InodeBlock newFileBlock);
void * diskController(void * args);
Block * getBlockData(int blockNumber, FILE * disk);
Block * getInodeBlock(int blockNumber, FILE * disk);



#endif

#include <string>
#include <fstream>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <sstream>
#include <string.h>
#include "Inode.h"
#include "diskController.h"
#include "diskOp.h"
#include "Common.h"
using namespace std;

int *freeBlockList;
int blockSize = 1;
int numBlocks = 0;

void readSuperBlock(FILE *disk){
	//printf("READING SUPERBLOCK\n");
	fread(&sb1, sizeof(SB), 1, disk);
	numBlocks = sb1.numBlocks;
	blockSize = sb1.blockSize;
}

void writeBlock(Block b,FILE* disk){
	fseek(disk, sb1.blockSize * b.blockNumber, SEEK_SET);
	fwrite(b.data,sb1.blockSize,1,disk);
}

void writeInodeBlock(InodeBlock b, int inodeNumber, FILE * disk){
	fseek(disk, sb1.blockSize * inodeNumber, SEEK_SET);
	fwrite(&b, sizeof(InodeBlock), 1, disk);
}


void readInodeMap(FILE *disk){
	//printf("READING INODES\n");
	int i = 0;
	for(i = 1; i < 257; i++){
		Bmap[i-1] = *(getInodeBlock(i,disk));
	}
	updateIMap();
}
void readFreeBlockList(FILE *disk){
	for(int i = 0; i < sb1.numBlocks; i++){
		if(i <= 257) freeList[i] = 1;
		else{
			fseek(disk, blockSize * i, SEEK_SET);
			fread(&freeList[i], sizeof(bool), 1, disk);
		}
	}
}

void writeInodeMap(FILE *disk){
	//printf("WRITING INODES\n");
	fseek(disk,blockSize,SEEK_SET);
	int i = 0;
	for(i = 0;i < 256;i++){
		fseek(disk, blockSize * (i + 1) ,SEEK_SET);
		fwrite(&Bmap[i].data,sizeof(InodeBlock),1,disk);
	}
}
void writeFreeBlockList(FILE * disk){
	for(int i = 258; i < sb1.numBlocks; i++){
		fseek(disk, blockSize * i, SEEK_SET);
		fwrite(&freeList[i], sizeof(char), 1, disk);
	}
}

Block * getBlockData(int blockNumber,FILE *disk){
	Block * b = new Block;
	fseek(disk,sb1.blockSize * blockNumber,SEEK_SET);
	b->blockSize = sb1.blockSize;
	b->blockNumber = blockNumber;
	b->data = (char *) malloc(sizeof(char) * sb1.blockSize);
	fread(b->data,sb1.blockSize,1,disk);
	return b;
}

Block * getInodeBlock(int blockNumber, FILE * disk){
	Block * b = new Block;
	InodeBlock d;
	fseek(disk, sb1.blockSize * blockNumber, SEEK_SET);
	b->blockSize = sb1.blockSize;
	b->blockNumber = blockNumber;
	fread(&d, sizeof(InodeBlock), 1, disk);
	//Dynamic setting
	b->data = (char *) malloc(sizeof(char) * sb1.blockSize);
	memcpy(b->data, d.name, sizeof(d.name));
	memcpy(b->data + sizeof(d.name), &d.fileSize, sizeof(d.fileSize));
	memcpy(b->data + sizeof(d.name) + sizeof(d.fileSize), &d.InUse, sizeof(d.InUse));
	memcpy(b->data + sizeof(d.name) + sizeof(d.fileSize) + sizeof(d.InUse), d.ptrs, sizeof(d.ptrs));
	return b;
}

/*Find a free Inode, and set the newFile inode into that spot on the disk*/
int UploadNewFile(FILE * disk, InodeBlock inodeBlock){
	int blockNum = -1;
	for(int i = 0; i < 256; i++){
		if(!(map[i].getInUse())) {
			blockNum = i+1;
			break;
		}
	}
	if(blockNum != -1) writeInodeBlock(inodeBlock, blockNum, disk);
	else perror("This shouldn't happen");
	readInodeMap(disk);

	return blockNum;
}

void consumeBuffer(FILE * disk){
	int i;
	while(start == tail){
		pthread_cond_wait(&filled, &mutex);
	}
	string command = buffer[tail];
	tail = (tail + 1) % MAX;
	pthread_cond_broadcast(&empty);
	pthread_mutex_unlock(&mutex);
	istringstream iss(command);
	string tmp;
	iss >> tmp;
	if(tmp == "CREATE"){
		iss >> tmp;
		CREATE(tmp, disk);
	}
	else if(tmp == "IMPORT"){
		iss >> tmp;
		string unixFile;
		iss >> unixFile;
		IMPORT(tmp, unixFile, disk);
	}
	else if(tmp == "CAT"){
		iss >> tmp;
		CAT(tmp, disk);
	}
	else if(tmp == "DELETE"){
		iss >> tmp;
		DELETE(tmp, disk);
	}
	else if(tmp == "WRITE"){
		string ssfsFile;
		string chars;
		string startByte;
		string numBytes;
		iss >> ssfsFile;
		iss >> chars;
		iss >> startByte;
		iss >> numBytes;
		int iNumBytes = stoi(numBytes);
		int iStartByte = stoi(startByte);
		WRITE(ssfsFile, chars, iStartByte, iNumBytes, 0, disk);
	}
	else if(tmp == "READ"){
		string ssfsFile;
		string startByte;
		string numBytes;
		iss >> ssfsFile;
		iss >> startByte;
		iss >> numBytes;
		int iNumBytes = stoi(numBytes);
		int iStartByte = stoi(startByte);
		READ(ssfsFile, iStartByte, iNumBytes, disk);
	}
	else if(tmp == "LIST"){
		LIST();
	}
	if(tmp != "SHUTDOWN"){
		consumeBuffer(disk);
	}
}

void * diskController(void * args){
	FILE *disk;
	//open with filename instead
	const char *fileName = (char *) args;
	disk = fopen(fileName,"rb+");
	//INITIALIZATION
	readSuperBlock(disk);
	readInodeMap(disk);
	readFreeBlockList(disk);
	consumeBuffer(disk);
	//ON SHUTDOWN COMMAND
	SHUTDOWN(disk);
}


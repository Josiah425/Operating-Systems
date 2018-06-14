#include <string.h>
#include <stdlib.h>
#include <fstream>
#include <pthread.h>
#include <iostream>
#include <unistd.h>
#include <string>
#include "Common.h"
#include "diskOp.h"
#include "diskController.h"
#include "Inode.h"

void * FileRequest(std::string command){
	int i;
	pthread_mutex_lock(&mutex);
	while((start + 1) % MAX == tail) pthread_cond_wait(&empty, &mutex);
	buffer[start] = command;
	start = (start + 1) % MAX;
	pthread_cond_broadcast(&filled);
	pthread_mutex_unlock(&mutex);
}

void * DiskOp(void * arg){
	char * fName = (char *) arg;
	std::string command;
	std::ifstream diskOp;
	diskOp.open(fName);
	while(!diskOp.eof()){
		getline(diskOp, command);
		if(command != ""){
			command += ' ';
			command += fName;
			FileRequest(command);
		}
	}
}

void updateIMap(){
	for(int i = 0; i < 256; i++){
		bool temp;
		int val;
		char name[32];
		memcpy(&name, &Bmap[i].data[0], 32);
		map[i].setFileName(name);
		memcpy(&val, &Bmap[i].data[32], sizeof(int));
		map[i].setFileSize(val);
		memcpy(&temp, &Bmap[i].data[36], sizeof(bool));
		map[i].setInUse(temp);
		//memcpy(&temp, &Bmap[i].data[37], sizeof(int));
		int dbptrs[12];
		for(int j = 0; j < 12; j++){
			memcpy(&dbptrs[j], &Bmap[i].data[37 + j], sizeof(int));
		}
		map[i].setDBPointers(dbptrs);
		memcpy(&val, &Bmap[i].data[81], sizeof(int));
		map[i].setIDBPointer(val);
		memcpy(&val, &Bmap[i].data[85], sizeof(int));
		map[i].setDIDBPointer(val);
	}
}

//DiskController
void CREATE(std::string ssfsFile, FILE * disk){
	std::cout << "Entering Create" << std::endl;
	int j = -1;
	for(int i = 0; i < 256; i++){
		if(map[i].getFileName() == ssfsFile){
			perror("File already exists.");
			return;
		}
	}
	for(int i = 0; i < 256; i++){
		if(!(map[i].getInUse())){
			j = i;
			break;
		}
	}
	if(j == -1) {
		perror("Currently 256 Files on disk.");
		return;
	}
	InodeBlock newFileBlock;
	strncpy(newFileBlock.name, ssfsFile.c_str(), 32);
	newFileBlock.fileSize = 0;
	newFileBlock.InUse = 1;
	int save = -1;
	for(int i = 257; i < sizeof(bool) * sb1.numBlocks; i++){
		if(freeList[i] == true){
			save = i;
			break;
		}
	}
	if(save == -1){
		perror("No more blocks to allocate");
		return;
	}
	newFileBlock.ptrs[0] = save;
	freeList[save] = 1;
	for(int i = 1; i < 14; i++) {
		newFileBlock.ptrs[i] = -1;
	}
	UploadNewFile(disk, newFileBlock);
}

//DiskController
void IMPORT(std::string ssfsFile, std::string unixFile, FILE * disk){
	std::cout << "Entering Import" << std::endl;
	int i;
	for(i = 0; i < 256; i++){
		if(map[i].getFileName() == unixFile){
			break;
		}
	}
	if(i == 256) CREATE(unixFile, disk);
	std::ifstream unixF;
	unixF.open(unixFile.c_str());
	std::string data = "";
	for(i = 0; unixF.eof() != true; i++) data += unixF.get();
	i--;
	data.erase(data.end()-1);
	WRITE(ssfsFile, data, 0, data.size(), 0, disk);
}
//DiskOp Do this but with blocks
void CAT(std::string ssfsFile, FILE * disk){
	std::cout << "Entering Cat" << std::endl;
	/*int i, j, blockIndex;
	blockIndex = 0;
	j = 0;
	for(i = 0; i < 256; i++){
		if(map[i].getFileName() == ssfsFile) break;
	}
	Block block = *(getBlockData(map[i].getDBPointerValue(blockIndex), disk));
	printf("%s", block.data);
	blockIndex++;
	int limit = 12 + (sb1.blockSize / sizeof(int *));
	int IDBPointerNum = 0;*/
	//TODO go through datablockpointers, then indirectblock ptr, then double indirect block ptr
	/*while(block.blockNumber != -1){
		if(blockIndex <= 12){
			block = getBlockData(map[i].getDBPointerValue(blockIndex), disk);
		}
		else if(blockIndex > 12 && blockIndex <= limit){
			block = getBlockData(map[i].getIDBPointer(), disk);
			int nextBlock;
			char data[4];
			for(int k = 0; k < 4; k++){
				data[k] = block.data[j];
				j++;
			}
			memcpy(&nextBlock, data, 4);
			block = getBlockData(nextBlock, disk);
		}
		//else{
			
		//}
		printf("%s", block.data);
		blockIndex++;
	}*/
}
//DiskController
void DELETE(std::string ssfsFile, FILE * disk){
	std::cout << "Entering delete: " << std::endl;
	int i = -1;
	int mapIndex;
	for(i = 0;i < 256;i++){
		if(map[i].getFileName() == ssfsFile){
			mapIndex = i;
			break;
		}
	}
	if(i == -1) {
		perror("File didn't exist to delete");
		exit(1);
	}
	InodeBlock del;
	strncpy(del.name, "", 32);
	del.fileSize = 0;
	del.InUse = 0;
	for(int j = 0; j < 12;j++){
		del.ptrs[j] = 0;
	}
	del.ptrs[12] = 0;
	del.ptrs[13] = 0;
	writeInodeBlock(del,i+1,disk);
	readInodeMap(disk);
}
//DiskController
void WRITE(std::string ssfsFile, std::string chars, int startByte, int numBytes, int update, FILE * disk){
	std::cout << "Entering Write" << std::endl;
	int i = 0;
	int mapIndex;
	for(i = 0;i<256;i++){
		if(map[i].getFileName() == ssfsFile){
			mapIndex = i;
			break;
		}
	}
	
	int startBlock = startByte / sb1.blockSize;
	int firstByte = startByte - (startBlock * sb1.blockSize);
	int amountOfBytes = sb1.blockSize - firstByte;
	char * printVal = (char *) malloc(sizeof(char) * amountOfBytes);
	Block dataBlock;
	dataBlock.data = (char *) malloc(sizeof(char) * sb1.blockSize);
	if(startBlock < 12){
		dataBlock = *(getBlockData(map[i].getDBPointerValue(startBlock), disk));
	}
	else if(startBlock >=13 && startBlock < sb1.blockSize / sizeof(int) + 13){
		int block;
		dataBlock = *(getBlockData(map[i].getIDBPointer(), disk));
		memcpy(&block, &dataBlock.data[(startBlock * 4) - (12 * sizeof(int))], sizeof(int));
		dataBlock = *(getBlockData(map[i].getDBPointerValue(block), disk));
	}
	for(int i = firstByte; i < amountOfBytes; i++) {
		dataBlock.data[i] = chars[update];
		update++;
	}
	writeBlock(dataBlock, disk);
	//WRITE(ssfsFile, chars, startByte, numBytes, update, disk);
}
//DiskController
void READ(std::string ssfsFile, int startByte, int numBytes, FILE * disk){
	std::cout << "Entering Read" << std::endl;
	int i = -1;
	int mapIndex;
	for(i = 0;i<256;i++){
		if(map[i].getFileName() == ssfsFile){
			mapIndex = i;
			break;
		}
	}
	if(i == -1) {
		perror("File not Found to Read");
		return;
	}
	char display;
	int startBlock = startByte / sb1.blockSize;
	int firstByte = startByte - (startBlock * sb1.blockSize);
	int amountOfBytes;
	if(numBytes > sb1.blockSize) amountOfBytes = sb1.blockSize - firstByte;
	else amountOfBytes = numBytes;
	char * printVal = (char *) malloc(sizeof(char) * amountOfBytes);
	Block dataBlock;
	dataBlock.data = (char *) malloc(sizeof(char) * sb1.blockSize);
	if(startBlock < 12){
		dataBlock = *(getBlockData(map[i].getDBPointerValue(startBlock), disk));
	}
	else if(startBlock >=13 && startBlock < sb1.blockSize / sizeof(int) + 13){
		int block;
		dataBlock = *(getBlockData(map[i].getIDBPointer(), disk));
		memcpy(&block, &dataBlock.data[(startBlock * 4) - (12 * sizeof(int))], sizeof(int));
		dataBlock = *(getBlockData(map[i].getDBPointerValue(block), disk));
	}
	if(firstByte == 0) firstByte = 1;
	memcpy(printVal, &dataBlock.data[firstByte], amountOfBytes);
	std::cout << printVal << std::endl;
	if(numBytes-amountOfBytes > 0) READ(ssfsFile, sb1.blockSize * startBlock, numBytes - amountOfBytes, disk);
}

void LIST(){
	std::cout << "Entering List" << std::endl;
	for(int i = 0; i < 256; i++) if(map[i].getInUse()) std::cout << map[i].getFileName() << std::endl;
}

void SHUTDOWN(FILE * disk){
	std::cout << "Entering Shutdown" << std::endl;
	writeInodeMap(disk);
	writeFreeBlockList(disk);
	exit(1);
}

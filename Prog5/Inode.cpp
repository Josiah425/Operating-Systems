#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "Inode.h"

void Inode::setFileName(std::string fileName){
	strncpy(this->fileName, fileName.c_str(), sizeof(this->fileName));
}

std::string Inode::getFileName(){
	return this->fileName;
}

void Inode::setFileSize(int fileSize){
	this->fileSize = fileSize;
}

int Inode::getFileSize(){
	return this->fileSize;
}

void Inode::setDBPointers(int DBPointers[12]){
	for(int i = 0; i < 12; i++){
		this->DBPointers[i] = DBPointers[i];
	}
}

int * Inode::getDBPointers(){
	return this->DBPointers;
}

int Inode::getDBPointerValue(int index){
	return this->DBPointers[index];
}

void Inode::setIDBPointer(int IDBPointer){
	this->IDBPointer = IDBPointer;
}

int Inode::getIDBPointer(){
	return this->IDBPointer;
}

void Inode::setDIDBPointer(int DIDBPointer){
	this->DIDBPointer = DIDBPointer;
}

int Inode::getDIDBPointer(){
	return this->DIDBPointer;
}

void Inode::setInUse(bool val){
	this->InUse = val;
}

bool Inode::getInUse(){
	return this->InUse;
}

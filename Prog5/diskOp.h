#ifndef DISKOP_H
#define DISKOP_H

#include <string>
#include "Common.h"

void * DiskOp(void * arg);
std::string ConsumeBuffer();
void * FileRequest(std::string command);
void CREATE(std::string ssfsFile, FILE * disk);
void IMPORT(std::string ssfsFile, std::string unixFile, FILE * disk);
void CAT(std::string ssfsFile, FILE * disk);
void DELETE(std::string ssfsFile, FILE * disk);
void WRITE(std::string ssfsFile, std::string chars, int startByte, int numBytes, int update, FILE * disk);
void READ(std::string ssfsFile, int startByte, int numBytes, FILE * disk);
void updateIMap();
void LIST();
void SHUTDOWN(FILE * disk);

#endif

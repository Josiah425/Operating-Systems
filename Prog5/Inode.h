#ifndef INODE_H
#define INODE_H

#include <string>

class Inode{
	private:
		char fileName[32];
		int fileSize;
		int DBPointers[12];
		int IDBPointer;
		int DIDBPointer;
		bool InUse;

	public:
		void setFileName(std::string fileName);
		std::string getFileName();
		void setFileSize(int fileSize);
		int getFileSize();
		void setDBPointers(int DBPointers[12]);
		int * getDBPointers();
		int getDBPointerValue(int index);
		void setIDBPointer(int IDBPointer);
		int getIDBPointer();
		void setDIDBPointer(int DIDBPointer);
		int getDIDBPointer();
		void setInUse(bool val);
		bool getInUse();
};

#endif

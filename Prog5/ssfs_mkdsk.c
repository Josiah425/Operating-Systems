#include <stdlib.h>
#include <stdio.h>

typedef struct SB{
	int numBlocks;
	int blockSize;
	char * name;
} SB;

int main(int argc, char * argv[]){
	if((atoi(argv[1]) < 1024 || atoi(argv[1]) > 131072) || (0 != (1 << 17) % atoi(argv[1]))){
		printf("Error, number of blocks must be between 1024 and 131072 and must be a power of 2.");
		exit(1);
	}
	if((atoi(argv[2]) < 128 || atoi(argv[2]) > 512) || (0 != (1 << 9) % atoi(argv[2]))){
		printf("Error, block size must be between 128 and 512, and must be a power of 2.");
		exit(1);
	}
	int numBlocks = atoi(argv[1]);
	int remainingBlocks = numBlocks;
	int blockSize = atoi(argv[2]);
	char * fileName;
	int i;
	if(argc == 4){
		fileName = argv[3];
	}
	else{
		fileName = "DISK";
	}

	int inodeMap = -1;
	int freeBlockList = -1;
	int tmp = 0;
	FILE * disk;
	disk = fopen(fileName, "wb");
	SB sb;
	sb.numBlocks = numBlocks;
	sb.blockSize = blockSize;
	sb.name = fileName;
	int size = numBlocks * blockSize - 1;
	fseek(disk, size, SEEK_SET);
	fputc('\0', disk);
	fseek(disk, 0, SEEK_SET);
	fwrite(&sb, sizeof(SB), 1, disk);
	fclose(disk);
	disk = fopen(fileName, "rb");
	SB sb1;
	fread(&sb1, sizeof(SB), 1, disk);
	printf("%d %d %s\n", sb1.numBlocks, sb1.blockSize, sb1.name);
	

	return 0;
}

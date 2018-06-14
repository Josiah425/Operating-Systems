#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <sys/time.h>
#include <stdbool.h>
#include <string.h>

int sizeOfData = 10000;
int cacheSize = 0;
int looping = 50;
int * cacheTable;
int * pages;

void clearCache(){
	int i;
	for(i = 0; i < cacheSize; i++){
		cacheTable[i] = -1;
	}
}

int getFreeIndex(){
	int i = 0;
	for(i = 0; i < cacheSize; i++){
		if(cacheTable[i] == -1) return i;
	}
	return i;
}

bool checkIfFree(){
	int i;
	for(i = 0; i < cacheSize; i++){
		if(cacheTable[i] == -1) return true;
	}
	return false;

}

int getIndex(int pageNumber){
	int j = 0;
	int returnVal = 0;
	for(j = 0; j < cacheSize; j++) if(cacheTable[j] == pageNumber) return j;
	return returnVal;
}

bool inTable(int pageNumber){
	int j;
	for(j = 0; j < cacheSize; j++){
		if(cacheTable[j] == pageNumber) return true;
	}
	return false;
}

int randomP(){
	int miss = 0;
	int cacheIndex;
	int j;

	for(j = 0; j < sizeOfData; j++){
		cacheIndex = (rand() % cacheSize);
		if(!inTable(pages[j])){
			miss++;
			if(checkIfFree()){
				miss--;
				cacheTable[getFreeIndex()] = pages[j];
			}
			else cacheTable[cacheIndex] = pages[j];
		}
	}
	clearCache();
	return miss;
}

int optimal(){
	int miss = 0;
	int j;
	int i;
	int k;
	int furthestIndex = 0;
	int * dontReplace = malloc(sizeof(int) * cacheSize);
	int issued = 0;
	for(i = 0; i < cacheSize; i++) dontReplace[i] = 0;
	for(i = 0; i < sizeOfData; i++){
		/*for(j = 0; j < cacheSize; j++){
			printf("%d ", cacheTable[j]);
		}
		printf("\n");*/
		if(!inTable(pages[i])){
			miss++;
			if(checkIfFree()) {
				miss--;
				furthestIndex = getFreeIndex();
			}
			else{
				for(j = i+1; j < sizeOfData; j++){
					for(k = 0; k < cacheSize; k++){
						if(pages[j] == cacheTable[k] && issued < cacheSize-1){
							dontReplace[k] = 1;
							issued++;
						}
					}
				}
				for(k = 0; k < cacheSize; k++){
					if(dontReplace[k] == 0){
						furthestIndex = k;
					}
					dontReplace[k] = 0;
				}
				issued = 0;
			}
			//printf("MISS: %d\nPage: %d\n", miss, pages[i]);
			cacheTable[furthestIndex] = pages[i];
		}
	}
	free(dontReplace);
	clearCache();
	return miss;
}

int LRU(){
	int miss = 0;
	int i;
	int j;
	int * time = malloc(sizeof(int) * cacheSize);
	int LRUIndex = 0;
	for(i = 0; i < cacheSize; i++){
		time[i] = cacheSize;
	}
	for(i = 0; i < sizeOfData; i++){
		if(!inTable(pages[i])){
			miss++;
			for(j = 0; j < cacheSize; j++){
				if(time[LRUIndex] < time[j]){
					LRUIndex = j;
				}
				time[j] = time[j] + 1;
			}
			if(cacheTable[LRUIndex] == -1) miss--;
			cacheTable[LRUIndex] = pages[i];
		}
		else{
			LRUIndex = getIndex(pages[i]);
			for(j = 0; j < cacheSize; j++){
				time[j] = time[j]+1;
			}
		}
		time[LRUIndex] = 0;
		LRUIndex = 0;
	}
	free(time);
	clearCache();
	return miss;
}

int FIFO(){
	int miss = 0;
	int i;
	int j;
	int * FIFOQueue = malloc(sizeof(int) * cacheSize);
	int index = 0;
	int count = 0;
	for(i = 0; i < cacheSize; i++){
		FIFOQueue[i] = -1;
	}
	for(i = 0; i < sizeOfData; i++){
		if(!inTable(pages[i])){
			miss++;
			count++;
			for(j = 0; j < cacheSize; j++){
				if(FIFOQueue[index] > FIFOQueue[j]){
					index = j;
				}
			}
			FIFOQueue[index] = count;
			if(cacheTable[index] == -1) miss--;
			cacheTable[index] = pages[i];
		}
		index = 0;
	}
	free(FIFOQueue);
	clearCache();
	return miss;
}

int clock(){
	int miss = 0;
	int i;
	int j;
	int * Clock = malloc(sizeof(int) * cacheSize);
	int index = 0;
	bool found = false;
	for(i = 0; i < cacheSize; i++){
		Clock[i] = 0;
	}
	for(i = 0; i < sizeOfData; i++){
		if(!inTable(pages[i])){
			miss++;
			while(!found){
				//Check if its set to 0
				if(Clock[index] == 0){
					if(cacheTable[index] == -1) miss--;
					cacheTable[index] = pages[i];
					Clock[index] = 1;
					found = true;
				}
				else{
					Clock[index] = 0;
				}
				index = (index+1) % cacheSize;
			}
		}
		else{
			Clock[getIndex(pages[i])] = 1;
		}
		found = false;
	}
	
	free(Clock);
	clearCache();
	return miss;
}

int main(int argc, char * argv[]){
	pages = malloc(sizeof(int) * sizeOfData);
	int seed = time(NULL);
	srand(seed);
	int minVal = 0;
	int maxVal = 99;
	int miss = 0;
	int cacheIndex = 0;
	int j;
	double hitRate;
	FILE * oFile;
	oFile = fopen("no-locality.csv", "w");
	fprintf(oFile, "#cache, OPT, LRU, FIFO, RAND, CLOCK\n");
	fprintf(oFile, "0, 0, 0, 0, 0, 0\n");
	//Initialize pages no-locality style
	for(j = 0; j < sizeOfData; j++){
		pages[j] = (rand() % (maxVal + 1 - minVal)) + minVal;
		//printf("%d\n", pages[j]);
	}
	cacheSize = 5;
	for(j = 0; j < 20; j++){
		fprintf(oFile, "%d, ", cacheSize);
		//printf("Cachesize: %d\n", cacheSize);
		cacheTable = malloc(sizeof(int) * cacheSize);
		clearCache();
		miss = optimal();
		hitRate = (100.0*(sizeOfData-miss)) / (sizeOfData);
		fprintf(oFile, "%f, ", hitRate);
		//printf("No-Locality Optimal\nHitRate: %f\n", hitRate);
		miss = LRU();
		hitRate = (100.0*(sizeOfData-miss)) / (sizeOfData);
		fprintf(oFile, "%f, ", hitRate);
		//printf("No-Locality Least Recently Used\nHitRate: %f\n", hitRate);
		miss = FIFO();
		hitRate = (100.0*(sizeOfData-miss)) / (sizeOfData);
		fprintf(oFile, "%f, ", hitRate);
		//printf("No-Locality First-In-First-Out\nHitRate: %f\n", hitRate);
		miss = randomP();
		hitRate = (100.0*(sizeOfData-miss)) / (sizeOfData);
		fprintf(oFile, "%f, ", hitRate);
		//printf("No-Locality Random\nHitRate: %f\n", hitRate);
		miss = clock();
		hitRate = (100.0*(sizeOfData-miss)) / (sizeOfData);
		fprintf(oFile, "%f\n", hitRate);
		//printf("No-Locality Clock\nHitRate: %f\n", hitRate);
		cacheSize += 5;
		//printf("\n");
		free(cacheTable);
	}
	fclose(oFile);
	oFile = fopen("80-20.csv", "w");
	fprintf(oFile, "#cache, OPT, LRU, FIFO, RAND, CLOCK\n");
	fprintf(oFile, "0, 0, 0, 0, 0, 0\n");
	//Initialize pages 80-20 style
	for(j = 0; j < sizeOfData; j++){
		int val = rand() % 100;
		if(val < 80) {
			pages[j] = (rand() % (20 + 1 - minVal)) + minVal;
		}
		else pages[j] = (rand() % (99 + 1 - 21)) + 21;
		//printf("%d ", pages[j]);
	}
	//printf("\n");

	cacheSize = 5;
	for(j = 0; j < 20; j++){
		fprintf(oFile, "%d, ", cacheSize);
		//printf("Cachesize: %d\n", cacheSize);
		cacheTable = malloc(sizeof(int) * cacheSize);
		clearCache();
		miss = optimal();
		hitRate = (100.0*(sizeOfData-miss)) / (sizeOfData);
		fprintf(oFile, "%f, ", hitRate);
		//printf("80-20 Optimal\nHitRate: %f\n", hitRate);
		miss = LRU();
		hitRate = (100.0*(sizeOfData-miss)) / (sizeOfData);
		fprintf(oFile, "%f, ", hitRate);
		//printf("80-20 Least Recently Used\nHitRate: %f\n", hitRate);
		miss = FIFO();
		hitRate = (100.0*(sizeOfData-miss)) / (sizeOfData);
		fprintf(oFile, "%f, ", hitRate);
		//printf("80-20 First-In-First-Out\nHitRate: %f\n", hitRate);
		miss = randomP();
		hitRate = (100.0*(sizeOfData-miss)) / (sizeOfData);
		fprintf(oFile, "%f, ", hitRate);
		//printf("80-20 Random\nHitRate: %f\n", hitRate);
		miss = clock();
		hitRate = (100.0*(sizeOfData-miss)) / (sizeOfData);
		fprintf(oFile, "%f\n", hitRate);
		//printf("80-20 Clock\nHitRate: %f\n", hitRate);
		cacheSize += 5;
		//printf("\n");
		free(cacheTable);
	}
	fclose(oFile);
	oFile = fopen("looping.csv", "w");
	fprintf(oFile, "#cache, OPT, LRU, FIFO, RAND, CLOCK\n");
	fprintf(oFile, "0, 0, 0, 0, 0, 0\n");

	//Initialize pages Looping style
	for(j = 0; j < sizeOfData; j++){
		pages[j] = j % looping;
	}

	cacheSize = 5;
	for(j = 0; j < 20; j++){
		fprintf(oFile, "%d, ", cacheSize);
		//printf("Cachesize: %d\n", cacheSize);
		cacheTable = malloc(sizeof(int) * cacheSize);
		clearCache();
		miss = optimal();
		hitRate = (100.0*(sizeOfData-miss)) / (sizeOfData);
		fprintf(oFile, "%f, ", hitRate);
		//printf("Looping Optimal\nHitRate: %f\n", hitRate);
		miss = LRU();
		hitRate = (100.0*(sizeOfData-miss)) / (sizeOfData);
		fprintf(oFile, "%f, ", hitRate);
		//printf("Looping Least Recently Used\nHitRate: %f\n", hitRate);
		miss = FIFO();
		hitRate = (100.0*(sizeOfData-miss)) / (sizeOfData);
		fprintf(oFile, "%f, ", hitRate);
		//printf("Looping First-In-First-Out\nHitRate: %f\n", hitRate);
		miss = randomP();
		hitRate = (100.0*(sizeOfData-miss)) / (sizeOfData);
		fprintf(oFile, "%f, ", hitRate);
		//printf("Looping Random\nHitRate: %f\n", hitRate);
		miss = clock();
		hitRate = (100.0*(sizeOfData-miss)) / (sizeOfData);
		fprintf(oFile, "%f\n", hitRate);
		//printf("Looping Clock\nHitRate: %f\n", hitRate);
		cacheSize += 5;
		//printf("\n");
		free(cacheTable);
	}
	fclose(oFile);
	clearCache();
	free(pages);

	return 0;
}

#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>
#include <fcntl.h>
#include <malloc.h>
#include <string.h>
#include <errno.h>

int main(int argc, char * argv[]){
	int minVal = 1;
	int minFlag = 0;
	int maxVal = 255;
	int maxFlag = 0;
	FILE *opF;
	int seed = time(NULL);
	int seedFlag = 0;
	int oFlag = 0;
	int sizeOfData = 100;
	int i = 0;
	int j = 0;
	for(j = 1; j < argc; j++){
		if(strcmp(argv[j],"-u") == 0){
			fprintf(stderr, "-n <num-integers>\n-m <min-int>\n-M <max-int>\n-s <seed>\n-o <output-file-name>\n");
			exit(1);
		}
		else if(strcmp(argv[j], "-n") == 0){
			if(argv[j+1][0] != '-'){
				if(atoi(argv[j+1]) <= 1000000){
					sizeOfData = atoi(argv[j+1]);
				}
				else{
					fprintf(stderr, "Value after -n flag must be less than or equal to 1,000,000\n");
					exit(1);
				}
			}
			else{
				fprintf(stderr, "-n <num-integers>\n-m <min-int>\n-M <max-int>\n-s <seed>\n-o <output-file-name>\n");
				exit(1);
			}
		}
		else if(strcmp(argv[j], "-m") == 0){
			if(argv[j+1][0] != '-'){
				if(atoi(argv[j+1]) >= 1){
					minVal = atoi(argv[j+1]);
					minFlag = 1;
				}
				else{
					fprintf(stderr, "Value after -m flag must be greater than or equal to 1\n");
					exit(1);
				}
			}
			else{
				fprintf(stderr, "-n <num-integers>\n-m <min-int>\n-M <max-int>\n-s <seed>\n-o <output-file-name>\n");
				exit(1);
			}
		}
		else if(strcmp(argv[j], "-M") == 0){
			if(argv[j+1][0] != '-'){
				if(atoi(argv[j+1]) <= 1000000){
					maxVal = atoi(argv[j+1]);
					maxFlag = 1;
				}
				else{
					fprintf(stderr, "Value after -M flag must be less than or equal to 1,000,000\n");
					exit(1);
				}
			}
			else{
				fprintf(stderr, "-n <num-integers>\n-m <min-int>\n-M <max-int>\n-s <seed>\n-o <output-file-name>\n");
				exit(1);
			}
		}
		else if(strcmp(argv[j], "-s") == 0){
			if(argv[j+1][0] != '-'){
				seed = atoi(argv[j+1]);
				seedFlag = 1;
			}
			else{
				fprintf(stderr, "-n <num-integers>\n-m <min-int>\n-M <max-int>\n-s <seed>\n-o <output-file-name>\n");
				exit(1);
			}
		}
		else if(strcmp(argv[j], "-o") == 0){
			if(argv[j+1][0] != '-'){
				opF = fopen(argv[j+1], "w");
				oFlag = 1;
			}
			else{
				fprintf(stderr, "-n <num-integers>\n-m <min-int>\n-M <max-int>\n-s <seed>\n-o <output-file-name>\n");
				exit(1);
			}
		}
		j++;
	}
	if(minVal > maxVal) {
		fprintf(stderr, "min value (%d) is greater than max value (%d)\n", minVal, maxVal);
		exit(1);
	}
	int * random = malloc(sizeof(int) * sizeOfData);
	srand(seed);
	for(j = 0; j < sizeOfData; j++){
		if(minFlag == 0 && maxFlag == 0){
			random[j] = rand();
		}
		else if(minFlag == 0 && maxFlag == 1){
			random[j] = rand() % (maxVal + 1);
		}
		else if(minFlag == 1 && maxFlag == 0){
			random[j] = rand() + minVal;
		}
		else{
			random[j] = (rand() % (maxVal + 1 - minVal)) + minVal;
		}
		if(oFlag == 0) fprintf(stdout, "%d\n", random[j]);
		else fprintf(opF, "%d\n", random[j]);
	}
	free(random);
	if(oFlag == 1) fclose(opF);

	return 0;
}

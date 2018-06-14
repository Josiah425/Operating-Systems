#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>
#include <fcntl.h>
#include <malloc.h>
#include <string.h>
#include "timer.c"

int compare(const void * a, const void * b){
	return *(int *)a - *(int *)b;
}

int main(int argc, char**argv, char *envp[]){
	FILE *ipF;
	FILE *opF;
	FILE *countFile;
	char * buffer = malloc(sizeof(char) * 1024);
	int i = 0;
	int j = 0;
	char * name = envp[5] + 8; /*envp[1] works on my computer at home, but envp[5] works on lab computers*/
	int nameSize = 0;
	int sizeOfData = 100;
	int maxFlag = 0;
	int minFlag = 0;
	int inputFlag = 0;
	int cFlag = 0;
	int oFlag = 0;
	int maxVal = 255;
	int minVal = 1;
	for(j = 1; j < argc; j++){
		if(strcmp(argv[j],"-u") == 0){
			fprintf(stderr, "-n <num-integers>\n-m <min-int>\n-M <max-int>\n-i <input-file-name>\n-o <output-file-name>\n-c <count-file-name>\n");
			exit(1);
		}
		else if(strcmp(argv[j], "-n") == 0){
			if(argv[j+1][0] != '-') {
				if(atoi(argv[j+1]) <= 1000000){
					sizeOfData = atoi(argv[j+1]);
				}
				else{
					fprintf(stderr, "Value after -n flag must be less than or equal to 1,000,000\n");
					exit(1);
				}
			}
			else{
				fprintf(stderr, "-n <num-integers>\n-m <min-int>\n-M <max-int>\n-i <input-file-name>\n-o <output-file-name>\n-c <count-file-name>\n");
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
				fprintf(stderr, "-n <num-integers>\n-m <min-int>\n-M <max-int>\n-i <input-file-name>\n-o <output-file-name>\n-c <count-file-name>\n");
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
				fprintf(stderr, "-n <num-integers>\n-m <min-int>\n-M <max-int>\n-i <input-file-name>\n-o <output-file-name>\n-c <count-file-name>\n");
				exit(1);
			}
		}
		else if(strcmp(argv[j], "-i") == 0){
			if(argv[j+1][0] != '-'){
				ipF = fopen(argv[j+1], "r");
				inputFlag = 1;
			}
			else{
				fprintf(stderr, "-n <num-integers>\n-m <min-int>\n-M <max-int>\n-i <input-file-name>\n-o <output-file-name>\n-c <count-file-name>\n");
				exit(1);
			}
		}
		else if(strcmp(argv[j], "-o") == 0){
			if(argv[j+1][0] != '-'){
				opF = fopen(argv[j+1], "w");
				oFlag = 1;
			}
			else{
				fprintf(stderr, "-n <num-integers>\n-m <min-int>\n-M <max-int>\n-i <input-file-name>\n-o <output-file-name>\n-c <count-file-name>\n");
				exit(1);
			}
		}
		else if(strcmp(argv[j], "-c") == 0){
			if(argv[j+1][0] != '-'){
				countFile = fopen(argv[j+1], "w");
				cFlag = 1;
			}
			else{
				fprintf(stderr, "-n <num-integers>\n-m <min-int>\n-M <max-int>\n-i <input-file-name>\n-o <output-file-name>\n-c <count-file-name>\n");
				exit(1);
			}
		}
		else{
			fprintf(stderr, "-n <num-integers>\n-m <min-int>\n-M <max-int>\n-i <input-file-name>\n-o <output-file-name>\n-c <count-file-name>\n");
			exit(1);
		}
		j++;
	}
	struct timeval tvStart;
	tvStart = start();
	if(minVal > maxVal) {
		fprintf(stderr, "min value (%d) is greater than max value (%d)\n", minVal, maxVal);
		exit(1);
	}
	int * data = malloc(sizeof(int) * sizeOfData);

	for(j = 0; j < sizeof(strlen(name)); j++){
		if(!isdigit(name[j])){
			nameSize++;
		}
	}

	int occurance[nameSize];
	for(j = 0; j < nameSize; j++){
		occurance[j] = 0;
	}

	if(inputFlag == 0){
		for(j = 0; j < sizeOfData; j++){
			scanf("%d", &data[j]);
			if(minFlag == 1){
				if(data[j] < minVal){
					fprintf(stderr, "Data contains value (%d) less than min val specified (%d).\n", data[j], minVal);
					exit(1);
				}
			}
			if(maxFlag == 1){
				if(data[j] > maxVal){
					fprintf(stderr, "Data contains value (%d) greater than max val specified (%d).\n", data[j], maxVal);
					exit(1);
				}
			}
		}
	}
	else{
		/*Checks based off the -n flag whether or not the data size is right based on first line*/
		int check;
		fscanf(ipF, "%d", &check);
		if(check != sizeOfData) {
			fprintf(stderr, "The input file's first line doesn't match with the size of data specified by -n or default value.\n");
			exit(1);
		}
		while(fgets(buffer, sizeof buffer, ipF)){
			if(i > sizeOfData){
				fprintf(stderr, "The input file has too much input.\n");
				exit(1);
			}
			fscanf(ipF, "%d", &data[i]);
			i++;
		}
		/*Check if despite the first number stating how big the data is, whether or not there actually are the amount specified*/
		if(i < sizeOfData+1){
			fprintf(stderr, "The input file has not enough input.\n");
			exit(1);
		}
		fclose(ipF);
	}
	qsort(data, sizeOfData, sizeof(int), compare);
	if(oFlag == 0){
		for(j = 0; j < sizeOfData; j++){
			fprintf(stdout, "%d\n", data[j]);
		}
		fprintf(stdout, "\n");
	}
	else{
		int k = 0;
		for(j = 0; j < sizeOfData; j++){
			fprintf(opF, "%d\t", data[k]);
			k++;
		}
		fclose(opF);
	}
	int temp = 0;
	for(j = 0; j < nameSize; j++){
		temp = name[j];
		for(i = 0; i < sizeOfData; i++){
			if(data[i] == temp){
				occurance[j] += 1;
			}
		}
	}
	if(cFlag == 0){
		for(j = 0; j < nameSize; j++){
			if(!isdigit(name[j])){
				fprintf(stdout, "%c\t%d\t%d\n", name[j], name[j], occurance[j]);
			}
		}
	}
	else{
		for(j = 0; j < nameSize; j++){
			if(!isdigit(name[j])){
				fprintf(countFile, "%c\t%d\t%d\n", name[j], name[j], occurance[j]);
			}
		}
		fclose(countFile);
	}
	free(data);
	free(buffer);
	end(tvStart);

	return 0;
}

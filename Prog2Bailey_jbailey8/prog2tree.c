#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>

int main(int argc, char ** argv){
	int numLevels = 0;
	int numChildren = 1;
	int pFlag = 0;
	int sFlag = 0;
	int sleepTime = 1;
	int currLevel = 0;
	int leaf = 0;
	int nI = -1;
	int mI = -1;
	int j;
	int i;
	for(j = 1; j < argc; j++){
		if(strcmp(argv[j],"-u") == 0){
			fprintf(stderr, "-n <num-integers>\n-m <min-int>\n-M <max-int>\n-i <input-file-name>\n-o <output-file-name>\n-c <count-file-name>\n");
			exit(1);
		}
		else if(strcmp(argv[j], "-N") == 0){
			if(argv[j+1][0] == '-'){
				fprintf(stderr, "-n <num-integers>\n-m <min-int>\n-M <max-int>\n-i <input-file-name>\n-o <output-file-name>\n-c <count-file-name>\n");
				exit(1);
			}
			if(atoi(argv[j+1]) > 4){
				fprintf(stderr, "Number of levels can be no larger than 4.");
				exit(1);
			}
			numLevels = atoi(argv[j+1]);
			nI = j+1;
		}
		else if(strcmp(argv[j], "-M") == 0){
			if(argv[j+1][0] == '-'){
				fprintf(stderr, "-n <num-integers>\n-m <min-int>\n-M <max-int>\n-i <input-file-name>\n-o <output-file-name>\n-c <count-file-name>\n");
				exit(1);
			}
			if(atoi(argv[j+1]) > 3){
				fprintf(stderr, "Number of children can be no larger than 3.");
				exit(1);
			}
			numChildren = atoi(argv[j+1]);
			mI = j+1;
		}
		else if(strcmp(argv[j], "-p") == 0){
			pFlag = 1;
			j--;
		}
		else if(strcmp(argv[j], "-s") == 0){
			if(argv[j+1][0] == '-'){
				fprintf(stderr, "-n <num-integers>\n-m <min-int>\n-M <max-int>\n-i <input-file-name>\n-o <output-file-name>\n-c <count-file-name>\n");
				exit(1);
			}
			sFlag = 1;
			sleepTime = atoi(argv[j+1]);
		}
		else if(strcmp(argv[j], "-leaf") == 0){
			leaf = 1;
		}
		else{
			fprintf(stderr, "-n <num-integers>\n-m <min-int>\n-M <max-int>\n-i <input-file-name>\n-o <output-file-name>\n-c <count-file-name>\n");
			exit(1);
		}
		j++;
	}
	if(getpid() == 0 && pFlag == 1) pause();
	if(sFlag == 0 && pFlag == 0) sFlag = 1;
	else if(sFlag == 1 && pFlag == 1) {
		fprintf(stderr, "Cannot set [-s] and [-p] flag at the same time.");
		fprintf(stderr, "-n <num-integers>\n-m <min-int>\n-M <max-int>\n-i <input-file-name>\n-o <output-file-name>\n-c <count-file-name>\n");
		exit(1);
	}

	pid_t pid;
	int flag = 1;
	if(numLevels != 0){
		numLevels -= 1;
		loop:
		for(i = 0; i < numChildren; i++){
			if(flag) {
				//original
				fprintf(stdout, "ALIVE: Level %d process with pid=%d, child of ppid=%d.\n", numLevels, getpid(), getppid());
				flag = 0;
				numLevels--;
			}
			pid = fork();
			if(pid == 0) {
				//child
				fprintf(stdout, "ALIVE: Level %d process with pid=%d, child of ppid=%d.\n", numLevels, getpid(), getppid());
				break;
			}
		}
		if(pid == 0 && numLevels--) {
			goto loop;
		}
		else if(pid == 0){
			if(pFlag == 1) pause();
			if(sFlag == 0){
				char * myargs[] = {argv[0], '\0'};
				execvp(myargs[0], myargs);
			}
			else{
				char buff[20];
				sprintf(buff, "%d", sleepTime);
				char * myargs[] = {argv[0], "-s", buff, '\0'};
				execvp(argv[0], myargs);
			}
		}
		else if(pid > 0) {
			while(1){
				int status;
				pid_t done = wait(&status);
				if(done == -1){
					if(errno == ECHILD) break;
				}
			}
			//parent
			fprintf(stdout, "EXITING: Level %d process with pid=%d, child of ppid=%d.\n", numLevels+1, getpid(), getppid());
		}
	}
	else{
		//leaf
		if(sFlag == 1) {
			sleep(sleepTime);
		}
		fprintf(stdout, "EXITING: Level %d process with pid=%d, child of ppid=%d.\n", numLevels, getpid(), getppid());
	}

	return 0;
}

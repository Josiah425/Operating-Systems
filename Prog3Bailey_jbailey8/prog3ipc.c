#include <stdio.h>
#include <stdlib.h>
#include <sys/signal.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>

int val;

void signal_handler(int sig){
	fprintf(stdout, "EXITING: Level %d process with pid=%d, child of ppid=%d.\n", val+2, getpid(), getppid());
	exit(0);
}

void child_function(int leaf){
	signal(SIGUSR1, signal_handler);
	if(leaf == 1) val = val-1;
	pause();
}

int main(int argc, char * argv[]){
	const char *name = "Jbailey8";
	int pfds[2];
	int i;
	int shm_fd;
	int leaf = 0;

	if(argc != 2) exit(1);
	if(atoi(argv[1]) < 1 || atoi(argv[1]) > 32){
		printf("Processes count must be between 1 and 32\n");
		exit(1);
	}
	int numProcesses = atoi(argv[1]);
	const int SIZE = sizeof(int) * numProcesses;
	val = numProcesses;
	pid_t pid;
	pid_t initPid = getpid();
	int * ptr;

	shm_fd = shm_open(name, O_CREAT | O_RDWR, 0666);
	ftruncate(shm_fd, SIZE);
	ptr = mmap(0, SIZE, PROT_WRITE, MAP_SHARED, shm_fd, 0);

	int fd;
	char * myfifo = "mypipe";
	mkfifo(myfifo, 0666);
	pid = getpid();
	for(i = 0; i < numProcesses; i++){
		if(pipe(pfds) == -1){
			perror("Error with pipe\n");
			exit(1);
		}
		pid = fork();
		if(pid != 0) {
			memcpy(ptr + i*sizeof(int), &pid, sizeof(int));
			break;
		}
		else{
			fprintf(stdout, "ALIVE: Level %d process with pid=%d, child of ppid=%d.\n", val, getpid(), getppid());		

			close(pfds[1]);

			if(read(pfds[0], &val, sizeof(int)) <= 0){
				perror("Error with child\n");
				exit(1);
			}
			if(val == 0){
				leaf = 1;
			}
		}
	}
	if(leaf == 1){
		memcpy(ptr + numProcesses*sizeof(int), &pid, sizeof(int));
		fd = open(myfifo, O_WRONLY);
		write(fd, "leaf", sizeof(int));
		close(fd);
		child_function(leaf);
	}
	if(pid > 0){
		close(pfds[0]);
		val = val-1;
		if(write(pfds[1], &val, sizeof(int)) <= 0){
			perror("Error with parent\n");
			exit(1);
		}
		if(initPid != getpid()) child_function(0);
	}
	if(initPid == getpid()){
		char buf[sizeof("leaf")];
		fd = open(myfifo, O_RDONLY);
		read(fd, buf, sizeof(int));
		for(i = 0; i < numProcesses; i++){
			printf("Level %d process: %d\n", numProcesses-i, *(ptr + i*sizeof(int)));
		}
		for(i = 0; i < numProcesses; i++){
			kill(*(ptr + i * sizeof(int)), SIGUSR1);
			sleep(2);
		}
		wait(NULL);
		close(fd);
		shm_unlink((char *) name);
		unlink(myfifo);
		exit(1);
	}
	
	return 0;
}

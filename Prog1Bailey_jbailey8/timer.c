#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>
#include <fcntl.h>
#include <malloc.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int timeval_subtract(struct timeval *result, struct timeval *t2, struct timeval *t1)
{
    long int diff = (t2->tv_usec + 1000000 * t2->tv_sec) - (t1->tv_usec + 1000000 * t1->tv_sec);
    result->tv_sec = diff / 1000000;
    result->tv_usec = diff % 1000000;

    return (diff<0);
}

struct timeval start(){
	struct timeval tvStart;
	gettimeofday(&tvStart, NULL);
	return tvStart;
}

struct timeval end(struct timeval tvStart){
	struct timeval tvDiff, tvEnd;
	gettimeofday(&tvEnd, NULL);
	timeval_subtract(&tvDiff, &tvEnd, &tvStart);
	fprintf(stderr, "%ld.%06ld\n", tvDiff.tv_sec, tvDiff.tv_usec);
}

#include <stdio.h>
#include <assert.h>
#include <pthread.h>
#include <malloc.h>

int bytesLeft = 256;

pthread_cond_t c;
pthread_mutex_t m;



void * allocate(int size){
	pthread_mutex_lock(&m);
	while(bytesLeft < size) pthread_cond_wait(&c, &m);
	void * ptr = malloc(sizeof(int) * bytesLeft);
	bytesLeft -= size;
	pthread_mutex_unlock(&m);
	return ptr;
}

void freeMem(void * ptr, int size){
	pthread_mutex_lock(&m);
	bytesLeft += size;
	pthread_cond_signal(&c);
	pthread_mutex_unlock(&m);

}

int main(int argc, char * argv[]){

	return 0;
}

#include <stdio.h>
#include <assert.h>
#include <pthread.h>
#include <stdint.h>

int buffer;
int count = 0;

void put(int value){
	assert(count == 0);
	count = 1;
	buffer = value;
}

int get(){
	assert(count == 1);
	count = 0;
	return buffer;
}

void * producer(void * arg){
	int i;
	int loops = (uintptr_t) arg;
	for(i = 0; i < loops; i++){
		put(i);
	}
}

void * consumer(void * arg){
	int i;
	while(1){
		int tmp = get();
		printf("%d\n", tmp);
	}
}

int main(int argc, char * argv[]){
	pthread_t p;
	pthread_t c;
	pthread_create(&p, NULL, producer, NULL);
	pthread_create(&c, NULL, consumer, NULL);
	pthread_join(p, NULL);
	pthread_join(c, NULL);
	return 0;
}

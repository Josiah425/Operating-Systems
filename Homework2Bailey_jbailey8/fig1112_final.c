#include <stdio.h>
#include <assert.h>
#include <pthread.h>

int MAX = 5;
int buffer[5] = {0};
int fill_ptr = 0;
int use_ptr = 0;
int count = 0;
pthread_cond_t empty, fill;
pthread_mutex_t mutex;
int loops = 5;

void put(int value){
	buffer[fill_ptr] = value;
	fill_ptr = (fill_ptr + 1) % MAX;
	count++;
}

int get(){
	int tmp = buffer[use_ptr];
	use_ptr = (use_ptr + 1) % MAX;
	count--;
	return tmp;
}

void * producer(void * arg){
	int i;
	for(i = 0; i < loops; i++){
		pthread_mutex_lock(&mutex);
		while(count == 1) pthread_cond_wait(&empty, &mutex);
		put(i);
		pthread_cond_signal(&fill);
		pthread_mutex_unlock(&mutex);
	}
}

void * consumer(void * arg){
	int i;
	for(i = 0; i < loops; i++){
		pthread_mutex_lock(&mutex);
		while(count == 0) pthread_cond_wait(&fill, &mutex);
		int tmp = get();
		pthread_cond_signal(&empty);
		pthread_mutex_unlock(&mutex);
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

#include <stdio.h> 
#include <stdlib.h> 
#include <pthread.h> 
#include <stdbool.h> 
// #include <string.h> remove the comment token if you want to double check

#define QSIZE	5 
#define LOOP	30 


typedef struct {
	int data[QSIZE]; 
	int index;
	int count; 
	pthread_mutex_t lock; 
	pthread_cond_t not_full; 
	pthread_cond_t not_empty; 
} queue_t; 

void *produce (void *args); 
void *consume (void *args); 
void put_data (queue_t *q, int d); 
int get_data (queue_t *q); 

bool queue_is_full(queue_t* q) {
	return (q->index == QSIZE);
}

bool queue_is_empty(queue_t* q) {
	return (q->index == 0); 
}


void put_data(queue_t* q, int d) {
	/*fill in here*/
	pthread_mutex_lock(&q->lock);

	while (q->count == QSIZE) {
		pthread_cond_wait(&q->not_full, &q->lock);
	}

	q->data[q->index] = d;
	q->index = (q->index + 1) % QSIZE;
	q->count++;

	pthread_cond_signal(&q->not_empty);
	pthread_mutex_unlock(&q->lock);
}

int get_data(queue_t* q) {
	int data; 
	/*fill in here */ 
	pthread_mutex_lock(&q->lock);

	while (q->count == 0) {
		pthread_cond_wait(&q->not_empty, &q->lock);
	}

	data = q->data[(q->index - q->count + QSIZE) % QSIZE];
	q->count--;

	pthread_cond_signal(&q->not_full);
	pthread_mutex_unlock(&q->lock);

	return data; 
}




queue_t *qinit() {
	queue_t *q;
	q = (queue_t *) malloc(sizeof(queue_t));
	// memset(q->data, -1, sizeof(q->data)); remove the comment token if you want to double check 
	q->index = q->count = 0; 
	pthread_mutex_init(&q->lock, NULL);
	pthread_cond_init(&q->not_full, NULL);
	pthread_cond_init(&q->not_empty, NULL); 
	return q; 
}

void qdelete(queue_t *q) {
	pthread_mutex_destroy(&q->lock);
	pthread_cond_destroy(&q->not_full); 
	pthread_cond_destroy(&q->not_empty); 
	free(q); 
}


void *produce(void* args) {
	int i, d; 
	queue_t *q = (queue_t *)args; 
	for (i = 0; i < LOOP; i++) {
		d = i; 
		put_data(q, d); 
		printf("put data %d to queue \n", d);
	}
	pthread_exit(NULL);
	return NULL; 
}


void *consume(void* args){
	int i, d; 
	queue_t *q = (queue_t *)args;
	for (i=0; i< LOOP; ++i) {
		d = get_data(q);
		printf("got data %d from queue\n", d); 
	}
	pthread_exit(NULL);
	return NULL; 

}



int main(void){
	queue_t * q; 
	pthread_t producer, consumer; 
	q = qinit(); 
	pthread_create(&producer, NULL, produce, (void *)q);
	pthread_create(&consumer, NULL, consume, (void *)q); 
	
	pthread_join(producer, NULL);
	pthread_join(producer, NULL);
	
	qdelete(q);

}

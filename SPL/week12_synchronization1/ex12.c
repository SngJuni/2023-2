#include <pthread.h> 
#include <stdlib.h>
#include <time.h> 
#include <stdio.h>
#include <sys/time.h>



pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
int results[2] = {0, 0}; // [0] = inside, [1] = all
int N_JOBS_PER_THREAD; 


void* monte_carlo_pi(void* rand_state) {
	unsigned int* state = (unsigned int*)rand_state;
	// xor multiple values together to get a semi-unique seed; 	
	*state = (unsigned long)rand_state ^ (unsigned long)pthread_self();
        /*FILL HERE  */	

	int histogram[2] = {0, 0};

	for (int i = 0; i < N_JOBS_PER_THREAD; ++i) {
		double x = (double)rand_r(state) / (double)RAND_MAX;
		double y = (double)rand_r(state) / (double)RAND_MAX;

		if (x*x + y*y <= 1.0) {
            histogram[0]++;
        }
		histogram[1]++;
	}

	pthread_mutex_lock(&lock);
    results[0] += histogram[0];
    results[1] += histogram[1];
	pthread_mutex_unlock(&lock);

	return NULL;
}


int main(int argc, char* argv[]){

	int n_threads = atoi(argv[1]);
	if (argc == 3)
		N_JOBS_PER_THREAD = atoi(argv[2]); 
	else
		N_JOBS_PER_THREAD = 100000;
	unsigned long* states = (unsigned long*)malloc(sizeof(unsigned long) * n_threads);
	// alloc memory 
	pthread_t* threads = (pthread_t*)malloc(sizeof(pthread_t) * n_threads); 
	
	time_t start_t = time(NULL);
	
	for(int i=0; i < n_threads; ++i) {
		pthread_create(&(threads[i]), NULL, &(monte_carlo_pi), &states[i]);
	}
	for(int i=0; i < n_threads; ++i) {
		pthread_join(threads[i], NULL); 
	}
	free(threads); 
	free(states); 
	// ratio results[0] / results[1] = pi/4
	time_t end_t = time(NULL); 

	double inside = results[0]; 
	double all = results[1]; 
	printf("%lf/%lf  pi is approximately, %lf", inside, all, 4.*inside/all ); 

}

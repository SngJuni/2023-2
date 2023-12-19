#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>


typedef struct {
	/* You need to design here */ 
    int M;
    int N;
    int cur;
    int* matrix;
    int* vector;
    int* result;
} thread_data; // you can change the name! 


void* multiply(void* arg) {
	/* You need to fill in here  */ 
    thread_data* data = (thread_data*) arg;
    
    int mul_sum = 0;

    for (int i = 0; i < data->N; i++) {
        mul_sum += data->matrix[i] * data->vector[i];
    }

    data->result[data->cur] = mul_sum;

    pthread_exit(NULL);
}



int main(int argc, char* argv[]) {
    if (argc != 3) {
        printf("Usage: %s <M> <N>\n", argv[0]);
        return 1;
    }

    int M = atoi(argv[1]);
    int N = atoi(argv[2]);

    int** matrix = malloc(M * sizeof(int*));
    int* vector = malloc(N * sizeof(int));
    int* result = malloc(M * sizeof(int));
    // Seeding 
    srand(time(NULL));

    // Initialize matrix and vector with random values
    printf("*** Matrix ***\n");
    for (int i = 0; i < M; i++) {
        matrix[i] = malloc(N * sizeof(int));
        for (int j = 0; j < N; j++) {
            matrix[i][j] = rand() % 10;
            printf("[ %d ] ", matrix[i][j]);
        }
        printf("\n");
    }

    printf("\n*** Vector ***\n");
    for (int i = 0; i < N; i++) {
        vector[i] = rand() % 10;
        printf("[ %d ]\n", vector[i]);
    }
    /*** From here you can change freely ***/ 
    
    // some initialization maybe? 
    pthread_t* threads = malloc(M * sizeof(pthread_t));
    thread_data* data = malloc(M * sizeof(thread_data));

    for (int i = 0; i < M; i++) {
	// Create threads, assign variables, and so on 
        data[i].M = M;
        data[i].N = N;
        data[i].matrix = matrix[i];
        data[i].vector = vector;
        data[i].result = result;
        data[i].cur = i;
        pthread_create(&threads[i], NULL, multiply, &data[i]);
    }

    // Fill the function to wait for all threads to complete
    for (int i = 0; i < M; i++) {
        pthread_join(threads[i], NULL);
    }

    // Print the result
    printf("\n *** Results ***\n");
    for (int i = 0; i < M; i++) {
        printf("[ %d ]\n", result[i]);
    }

    /*** This part is just clean up  ***/
    // Clean up
    for (int i = 0; i < M; i++) {
        free(matrix[i]);
    }
    free(matrix);
    free(vector);
    free(result);
    free(threads);
    free(data);

    return 0;
}


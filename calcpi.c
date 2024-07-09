// GRR19891218 Carlos Matheus


#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>

#define TERMS 1000000000
#define NUM_THREADS 4

double partial_sums[NUM_THREADS];

void *calculate_pi(void *arg) {
    int thread_id = *((int *) arg);
    double partial_sum = 0.0;
    int start = thread_id * (TERMS / NUM_THREADS);
    int end = start + (TERMS / NUM_THREADS);
    
    for (int i = start; i < end; i++) {
        double numerator = pow(-1, i);
        double denominator = 2 * i + 1;
        partial_sum += numerator / denominator;
    }
    
    partial_sums[thread_id] = partial_sum;
    pthread_exit(NULL);
}

int main() {
    pthread_t threads[NUM_THREADS];
    int thread_ids[NUM_THREADS];
    void *status;
    double total_sum = 0.0;
    struct timespec start_time, end_time;
    double elapsed_time;

    clock_gettime(CLOCK_MONOTONIC, &start_time);
    
    for (int i = 0; i < NUM_THREADS; i++) {
        thread_ids[i] = i;
        pthread_create(&threads[i], NULL, calculate_pi, (void *) &thread_ids[i]);
    }
    
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], &status);
        total_sum += partial_sums[i];
    }
    
    double pi = 4 * total_sum;
    
    clock_gettime(CLOCK_MONOTONIC, &end_time);
    elapsed_time = (end_time.tv_sec - start_time.tv_sec) +
                   (end_time.tv_nsec - start_time.tv_nsec) / 1e9;
    
    printf("Estimated value of π is %.15lf\n", pi);
    printf("Time taken: %.6lf seconds\n", elapsed_time);
    
    return 0;
}

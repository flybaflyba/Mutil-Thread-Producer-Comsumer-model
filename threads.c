
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

void *do_math(void * arg); //void * a pointer that can point to anything

int sum;

int main(int argc, char **argv) {
    
    int num_threads;
    sum = 0;
    pthread_t * threads;
    
    if (argc < 2) {
        printf("usage: %s [NUM ITERATIONS]\n", argv[0]);
        return 0;
    }
    
    num_threads = atoi(argv[1]);
    
    threads = (pthread_t*) calloc(num_threads, sizeof(pthread_t));
    
    printf("Launching %d threads\n", num_threads);

    for (int i=0; i<num_threads; i++) {
        long thread_id = (long)i;
        if (pthread_create(&threads[i], NULL, &do_math, (void *)thread_id) != 0) {
            perror("Error");
        }
    }
    
    // keep the threads running after main thread finished
    for (int i=0; i<num_threads; i++) {
        pthread_join(threads[i], NULL);
    }
    
    printf("Sum = %d\n", sum);
    
   
    
    return 0;
}

// all threads can access to sum, becasue sum is global variable 

void *do_math(void *arg) {
    int t_id = (int)(long)arg;
    int temp;
    printf("Thread %d launched ...\n", t_id);
    for (int n=0; n<100; n++) {
        temp = sum;
        usleep(5);
        temp++;
        sum = temp;
    }
}
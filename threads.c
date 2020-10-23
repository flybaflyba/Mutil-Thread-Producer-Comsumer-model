
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h> // allows more than 1 threads to access, we dont do this here though // mutex can only be 0 or one 

void *do_math(void * arg); //void * a pointer that can point to anything

void *produce(void * arg); 

void *consume(void * arg); 

int sum;
sem_t sum_lock;

int A;
sem_t a_lock;

int B;
sem_t b_lock;

int main(int argc, char **argv) {
    
    int num_threads;
    sum = 0;
    pthread_t * threads;
    A = 0;
    B = 0;
    
    if (argc < 2) {
        printf("usage: %s [NUM ITERATIONS]\n", argv[0]);
        return 0;
    }
    
    num_threads = atoi(argv[1]);
    
    threads = (pthread_t*) calloc(num_threads, sizeof(pthread_t));
    
    if (sem_init(&sum_lock, 0, 1) < 0) { // 0: not share with other process, 1: one thread to go through at one time
        perror("Error initizing sum_lock");
        return 0;
    }
    if (sem_init(&a_lock, 0, 1) < 0) { 
        perror("Error initizing a_lock");
        return 0;
    }
    if (sem_init(&b_lock, 0, 1) < 0) { 
        perror("Error initizing b_lock");
        return 0;
    }
    
    
    printf("Launching %d threads\n", num_threads);

    for (int i=0; i<num_threads/2; i++) {
        //printf("------ %d \n", i);
        
        long thread_id_consumer = (long)(num_threads - i);
        if (pthread_create(&threads[num_threads - i], NULL, &consume, (void *)thread_id_consumer) != 0) {
            perror("Error");
        }
        long thread_id_producer = (long)i;
        if (pthread_create(&threads[i], NULL, &produce, (void *)thread_id_producer) != 0) {
            perror("Error");
        }
     
    }
    
    // keep the threads running after main thread finished
    for (int i=0; i<=num_threads; i++) {
        pthread_join(threads[i], NULL); // the i here is what we set above, becasue I used num_threads - i and i can be 0, so here I need to use <= 
    }
    
    //printf("Sum = %d\n", sum);
    printf("A = %d\n", A);
    printf("B = %d\n", B);
    
    
    return 0;
}

// all threads can access to sum, becasue sum is global variable 

void *do_math(void *arg) {
    int t_id = (int)(long)arg;
    int temp;
    printf("Thread %d launched ...\n", t_id);
    for (int n=0; n<100; n++) {
        sem_wait(&sum_lock); // sum_lock is decremented 1 to 0 now, no thread can get in
        temp = sum; // critical section, 4 lines 
        usleep(5);
        temp++;
        sum = temp; 
        sem_post(&sum_lock); // sum_lock is incremented 1 to 1 now, one threaad can get in 
    }
}




void *produce(void *arg) {
    int t_id = (int)(long)arg;
    int temp;
    printf("Producer thread %d launched ...\n", t_id);
   
 
    
    //printf("P finished\n");
    
}

void *consume(void *arg) {
    int t_id = (int)(long)arg;
    int temp;
    printf("Consumer thread %d launched ...\n", t_id);
    
    
    
    //printf("C finished\n");
    
}


// void *produce(void *arg) {
//     int t_id = (int)(long)arg;
//     int temp;
//     printf("Producer thread %d launched ...\n", t_id);
   
//     sem_wait(&a_lock);
//     A = A + 1;
//     //printf("PA--%d\n", A);
//     usleep(5);
//     sem_post(&a_lock);
    
//     sem_wait(&b_lock);
//     B = B + 2; 
//     //printf("PB--%d\n", B);
//     usleep(5);
//     sem_post(&b_lock);
    
//     //printf("P finished\n");
    
// }

// void *consume(void *arg) {
//     int t_id = (int)(long)arg;
//     int temp;
//     printf("Consumer thread %d launched ...\n", t_id);
    
//     sem_wait(&b_lock);
//     B = B + 3; 
//     //printf("CB--%d\n", B);
//     usleep(5);
//     sem_post(&b_lock);

//     sem_wait(&a_lock);
//     A = A + 1; 
//     //printf("CA--%d\n", A);
//     usleep(5);
//     sem_post(&a_lock);
    
//     //printf("C finished\n");
    
// }
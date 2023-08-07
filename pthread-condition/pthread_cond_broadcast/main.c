#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define NUM_THREADS 3

pthread_mutex_t mutex;
pthread_cond_t cond_var;
int shared_data = 0;

void *thread_function(void *thread_id)
{
    long tid = (long)thread_id;

    pthread_mutex_lock(&mutex);

    while(shared_data < 10)
    {
        printf("Thread %ld is waiting...\n", tid);
        pthread_cond_wait(&cond_var, &mutex);
        printf("Thread %ld is awake and running. Shared data: %d\n", tid, shared_data);
    }

    pthread_mutex_unlock(&mutex);
    pthread_exit(NULL);
}

int main(void)
{
    pthread_t threads[NUM_THREADS];
    int rc;
    long t;

    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&cond_var, NULL);

    for (t = 0; t < NUM_THREADS; t++)
    {
        rc = pthread_create(&threads[t], NULL, thread_function, (void *)t);

        if(rc)
        {
            printf("ERROR: Return code from pthread_create() is %d\n", rc);
            exit(EXIT_FAILURE);
        }
    }

    // Simulate some work and update the shared_data variable
    for(int i = 0; i < 10; i++)
    {
        pthread_mutex_lock(&mutex);
        shared_data = i + 1;
        printf("Main thread updating shared_data: %d\n", shared_data);
        pthread_cond_broadcast(&cond_var);
        pthread_mutex_unlock(&mutex);
    }

    // Wait for all threads to finish
    for (t = 0; t < NUM_THREADS; t++) {
        pthread_join(threads[t], NULL);
    }

    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond_var);
    pthread_exit(NULL);
}

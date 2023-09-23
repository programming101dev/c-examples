/*
 * This code is licensed under the Attribution-NonCommercial-NoDerivatives 4.0 International license.
 *
 * Author: D'Arcy Smith (ds@programming101.dev)
 *
 * You are free to:
 *   - Share: Copy and redistribute the material in any medium or format.
 *   - Under the following terms:
 *       - Attribution: You must give appropriate credit, provide a link to the license, and indicate if changes were made.
 *       - NonCommercial: You may not use the material for commercial purposes.
 *       - NoDerivatives: If you remix, transform, or build upon the material, you may not distribute the modified material.
 *
 * For more details, please refer to the full license text at:
 * https://creativecommons.org/licenses/by-nc-nd/4.0/
 */


#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>


static void *thread_function(void *thread_id);
#define NUM_THREADS 3


static pthread_mutex_t mutex;


static pthread_cond_t  cond_var;


static int             shared_data = 0;


int main(void)
{
    pthread_t threads[NUM_THREADS];
    int       rc;
    long      t;
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&cond_var, NULL);
    for(t = 0; t < NUM_THREADS; t++)
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
#if defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wthread-safety-negative"
#endif
        pthread_mutex_lock(&mutex);
#if defined(__clang__)
#pragma clang diagnostic pop
#endif
        shared_data = i + 1;
        printf("Main thread updating shared_data: %d\n", shared_data);
        pthread_cond_broadcast(&cond_var);
        pthread_mutex_unlock(&mutex);
    }

    // Wait for all threads to finish
    for(t     = 0; t < NUM_THREADS; t++)
    {
        pthread_join(threads[t], NULL);
    }
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond_var);
    pthread_exit(NULL);
}


static void *thread_function(void *thread_id)
{
    long tid = (long)thread_id;
#if defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wthread-safety-negative"
#endif
    pthread_mutex_lock(&mutex);
#if defined(__clang__)
#pragma clang diagnostic pop
#endif
    while(shared_data < 10)
    {
        printf("Thread %ld is waiting...\n", tid);
        pthread_cond_wait(&cond_var, &mutex);
        printf("Thread %ld is awake and running. Shared data: %d\n", tid, shared_data);
    }
    pthread_mutex_unlock(&mutex);
    pthread_exit(NULL);
}

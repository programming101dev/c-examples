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

#include <inttypes.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct thread_data
{
    int             *sharedVariable;
    pthread_mutex_t *mutex;
};

static void *thread_function(void *arg);

#define NUM_THREADS 10

// TODO: is there a way to make it so that trylock doesn't succeed?

int main(void)
{
    pthread_mutex_t    mutex;
    pthread_t          threads[NUM_THREADS];
    int                sharedVariable;
    int                i;
    struct thread_data data;

    if(pthread_mutex_init(&mutex, NULL) != 0)
    {
        fprintf(stderr, "Error: Mutex initialization failed.\n");
        return EXIT_FAILURE;
    }

    sharedVariable = 0;    // Local shared variable for main thread

    // Create multiple threads
    data.sharedVariable = &sharedVariable;
    data.mutex          = &mutex;

    for(i = 0; i < NUM_THREADS; i++)
    {
        if(pthread_create(&threads[i], NULL, thread_function, (void *)&data) != 0)
        {
            fprintf(stderr, "Error: Thread creation failed.\n");
            return EXIT_FAILURE;
        }
    }

    // Wait for all threads to finish
    for(i = 0; i < NUM_THREADS; i++)
    {
        pthread_join(threads[i], NULL);
    }

    // Destroy the mutex after all threads are done using it
    pthread_mutex_destroy(&mutex);

    return EXIT_SUCCESS;
}

static void *thread_function(void *arg)
{
    pthread_t           tid;
    struct thread_data *data;

    data = (struct thread_data *)arg;

    // Attempt to lock the mutex before accessing the shared variable
    if(pthread_mutex_trylock(data->mutex) != 0)
    {
        // If the lock cannot be obtained, skip the critical section
        pthread_exit(NULL);
    }

    // Critical section: Accessing and modifying the shared variable
    (*(data->sharedVariable))++;

    // Get the thread ID
    tid = pthread_self();

    // Print the thread ID and shared variable value (using pointer representation for pthread_t)
    printf("Thread %lu: Shared variable value: %d\n", (unsigned long)tid, *(data->sharedVariable));

    // Unlock the mutex after finishing the critical section
    pthread_mutex_unlock(data->mutex);

    // Exit the thread
    pthread_exit(NULL);
}

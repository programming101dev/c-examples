#include <errno.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>


static void *thread_function(void *arg);


pthread_cond_t cond_var = PTHREAD_COND_INITIALIZER;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;


int main(void)
{
    pthread_t thread_id;
    struct timespec abs_timeout;
    int result;

    // Create the thread
    pthread_create(&thread_id, NULL, thread_function, NULL);

    // Get the current time and set the timeout to 5 seconds
    clock_gettime(CLOCK_REALTIME, &abs_timeout);
    abs_timeout.tv_sec += 5; // Changed to 5 seconds

    // Wait for the condition variable with a timeout of 5 seconds
    pthread_mutex_lock(&mutex);
    result = pthread_cond_timedwait(&cond_var, &mutex, &abs_timeout);
    pthread_mutex_unlock(&mutex);

    if(result == 0)
    {
        printf("Condition variable signaled before timeout.\n");
    }
    else if(result == ETIMEDOUT)
    {
        printf("Timeout occurred. Condition variable was not signaled.\n");
    }
    else
    {
        printf("Error while waiting for the condition variable.\n");
    }

    // Wait for the thread to finish
    pthread_join(thread_id, NULL);

    // Destroy mutex and condition variable
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond_var);

    return EXIT_SUCCESS;
}


#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"

static void *thread_function(void *arg)
{
    // Generate a random sleep time between 4 and 6 seconds
    srand((unsigned int) time(NULL));
    int sleep_time = 4 + rand() % 3; // Random value in [4, 6]

    printf("Thread will sleep for %d seconds.\n", sleep_time);
    sleep(sleep_time);

    // Signal the condition variable
    pthread_mutex_lock(&mutex);
    pthread_cond_signal(&cond_var);
    pthread_mutex_unlock(&mutex);

    return NULL;
}
#pragma GCC diagnostic pop

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>


static void *thread_function(void *arg);


int main(void)
{
    pthread_t thread1, thread2;
    pthread_create(&thread1, NULL, thread_function, NULL);
    pthread_create(&thread2, NULL, thread_function, NULL);

    // Check if thread1 and thread2 have the same thread ID
    if(pthread_equal(thread1, thread2))
    {
        printf("Thread 1 and Thread 2 have the same thread ID.\n");
    }
    else
    {
        printf("Thread 1 and Thread 2 have different thread IDs.\n");
    }

    // Check if thread1 has the same thread ID as itself
    if(pthread_equal(thread1, pthread_self()))
    {
        printf("Thread 1 and the main thread have the same thread ID.\n");
    }
    else
    {
        printf("Thread 1 and the main thread have different thread IDs.\n");
    }

    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    printf("Main thread is done.\n");

    return EXIT_SUCCESS;
}


#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
static void *thread_function(void *arg)
{
    return NULL;
}
#pragma GCC diagnostic pop

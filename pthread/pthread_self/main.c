#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>


static void *thread_function(void *arg);


int main(void)
{
    pthread_t threads[3];

    for(int i = 0; i < 3; i++)
    {
        pthread_create(&threads[i], NULL, thread_function, NULL);
    }

    for(int i = 0; i < 3; i++)
    {
        pthread_join(threads[i], NULL);
    }

    printf("Main thread is done.\n");
    return EXIT_SUCCESS;
}


#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"

static void *thread_function(void *arg)
{
    pthread_t thread_id = pthread_self();
    printf("Thread ID: %lu\n", (unsigned long) thread_id);

    // Perform some work in the thread...

    return NULL;
}
#pragma GCC diagnostic pop

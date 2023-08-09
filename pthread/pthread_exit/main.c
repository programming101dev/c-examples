#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>


void *thread_function(void *arg);


void *thread_function(void *arg)
{
    int thread_id = *(int*)arg;
    printf("Thread %d is executing.\n", thread_id);

    // Perform some work in the thread...

    printf("Thread %d is done.\n", thread_id);

    // Exit the thread and return a value (optional)
    int* result = malloc(sizeof(int));
    *result = thread_id * 2;
    pthread_exit(result);
}

int main(void)
{
    pthread_t threads[3];
    int thread_args[3] = {1, 2, 3};

    for(int i = 0; i < 3; i++)
    {
        pthread_create(&threads[i], NULL, thread_function, &thread_args[i]);
    }

    // Wait for all threads to finish and retrieve their exit values
    for(int i = 0; i < 3; i++)
    {
        int* result;
        pthread_join(threads[i], (void**)&result);
        printf("Thread %d returned: %d\n", i+1, *result);
        free(result); // Don't forget to free the memory returned by the thread
    }

    printf("Main thread is done.\n");

    return EXIT_SUCCESS;
}

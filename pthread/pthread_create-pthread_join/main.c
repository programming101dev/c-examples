#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>


static void *print_numbers(void *arg);


int main(void)
{
    pthread_t thread_id;
    int result;

    // Create the thread
    result = pthread_create(&thread_id, NULL, print_numbers, NULL);
    if(result != 0)
    {
        perror("Thread creation failed");
        return 1;
    }

    printf("Main is working\n");
    sleep(2);
    printf("Main is still working\n");
    sleep(2);
    printf("Main is done working, waiting for the thread to finish\n");

    // Wait for the thread to finish execution
    pthread_join(thread_id, NULL);

    printf("Thread execution completed.\n");

    return EXIT_SUCCESS;
}


#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"

static void *print_numbers(void *arg)
#pragma GCC diagnostic pop
{
    for(int i = 1; i <= 5; ++i)
    {
        printf("Thread: %d\n", i);
        sleep(1);
    }

    // The thread returns NULL upon completion
    return NULL;
}

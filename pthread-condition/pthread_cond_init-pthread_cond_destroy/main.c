#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>


int main(void)
{
    pthread_cond_t condition;

    // Initialize the condition variable
    int init_result = pthread_cond_init(&condition, NULL);

    if(init_result != 0)
    {
        fprintf(stderr, "Error initializing condition variable. Error code: %d\n", init_result);
        return EXIT_FAILURE;
    }

    // Do some main thread work here (not relevant for this example)

    // Destroy the condition variable
    int destroy_result = pthread_cond_destroy(&condition);

    if(destroy_result != 0)
    {
        fprintf(stderr, "Error destroying condition variable. Error code: %d\n", destroy_result);
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

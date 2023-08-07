#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>


int main(void)
{
    pthread_mutex_t mutex;

    // Initialize the mutex
    if(pthread_mutex_init(&mutex, NULL) != 0) {
        fprintf(stderr, "Error: Mutex initialization failed.\n");
        return 1;
    }

    // Use the mutex here (not shown in this example)

    // Destroy the mutex after it is no longer needed
    pthread_mutex_destroy(&mutex);

    return EXIT_SUCCESS;
}

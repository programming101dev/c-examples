#include <stdio.h>
#include <pthread.h>
#include <unistd.h> // For sleep function

#define NUM_THREADS 10

// Struct to hold shared data (shared variable and mutex)
struct thread_data
{
    int* sharedVariable;
    pthread_mutex_t* mutex;
};

// Function that multiple threads will execute
void* threadFunction(void* arg) {
    struct thread_data* data = (struct thread_data*)arg;

    // Attempt to lock the mutex before accessing the shared variable
    if (pthread_mutex_trylock(data->mutex) != 0)
    {
        // If the lock cannot be obtained, skip the critical section
        pthread_exit(NULL);
    }

    // Critical section: Accessing and modifying the shared variable
    (*(data->sharedVariable))++;
    printf("Thread %ld: Shared variable value: %d\n", (long)pthread_self(), *(data->sharedVariable));

    // Unlock the mutex after finishing the critical section
    pthread_mutex_unlock(data->mutex);

    // Exit the thread
    pthread_exit(NULL);
}

int main() {
    // Initialize the mutex
    pthread_mutex_t mutex;
    if (pthread_mutex_init(&mutex, NULL) != 0) {
        fprintf(stderr, "Error: Mutex initialization failed.\n");
        return 1;
    }

    // Create an array of threads
    pthread_t threads[NUM_THREADS];
    int sharedVariable = 0; // Local shared variable for main thread
    int i;

    // Create multiple threads
    struct thread_data data;
    data.sharedVariable = &sharedVariable;
    data.mutex = &mutex;

    for (i = 0; i < NUM_THREADS; i++)
    {
        if (pthread_create(&threads[i], NULL, threadFunction, (void*)&data) != 0)
        {
            fprintf(stderr, "Error: Thread creation failed.\n");
            return 1;
        }
    }

    // Wait for all threads to finish
    for (i = 0; i < NUM_THREADS; i++)
    {
        pthread_join(threads[i], NULL);
    }

    // Destroy the mutex after all threads are done using it
    pthread_mutex_destroy(&mutex);

    return 0;
}

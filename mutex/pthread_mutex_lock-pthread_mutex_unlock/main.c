#include <stdio.h>
#include <pthread.h>
#include <stdlib.h> // For atoi
#include <getopt.h> // For getopt

#define NUM_THREADS 10

// Struct to hold shared data (shared variable and mutex)
struct thread_data
{
    int* sharedVariable;
    pthread_mutex_t* mutex;
    int useMutex;
};

void *threadFunction(void* arg);
void printHelp(void);

// Function that multiple threads will execute
void *threadFunction(void* arg)
{
    struct thread_data* data = (struct thread_data*)arg;

    if(data->useMutex) {
        // Lock the mutex before accessing the shared variable
        pthread_mutex_lock(data->mutex);
    }

    // Critical section: Accessing and modifying the shared variable
    (*(data->sharedVariable))++;
    printf("Thread %ld: Shared variable value: %d\n", (long)pthread_self(), *(data->sharedVariable));
    // sleep(1); // Introduce a 1-second delay

    if(data->useMutex) {
        // Unlock the mutex after finishing the critical section
        pthread_mutex_unlock(data->mutex);
    }

    // Exit the thread
    pthread_exit(NULL);
}

void printHelp(void)
{
    printf("Usage: ./program_name [-h] [-m]\n");
    printf("Options:\n");
    printf("  -h        Display this help message.\n");
    printf("  -m        Use mutex for threadFunction (default is no mutex).\n");
}

int main(int argc, char* argv[]) {
    int opt;
    int useMutex = 0; // Default is no mutex

    // Process command-line options
    while((opt = getopt(argc, argv, "hm")) != -1) {
        switch (opt) {
            case 'h':
                printHelp();
                return EXIT_SUCCESS;
            case 'm':
                useMutex = 1;
                break;
            default:
                fprintf(stderr, "Error: Unknown option.\n");
                return 1;
        }
    }

    // Initialize the mutex if needed
    pthread_mutex_t mutex;
    if(useMutex && pthread_mutex_init(&mutex, NULL) != 0) {
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
    data.useMutex = useMutex;

    for(i = 0; i < NUM_THREADS; i++)
    {
        if(pthread_create(&threads[i], NULL, threadFunction, (void*)&data) != 0)
        {
            fprintf(stderr, "Error: Thread creation failed.\n");
            return 1;
        }
    }

    // Wait for all threads to finish
    for(i = 0; i < NUM_THREADS; i++)
    {
        pthread_join(threads[i], NULL);
    }

    // Destroy the mutex if needed
    if(useMutex) {
        pthread_mutex_destroy(&mutex);
    }

    return EXIT_SUCCESS;
}

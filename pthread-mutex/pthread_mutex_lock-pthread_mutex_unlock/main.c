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


#include <stdio.h>
#include <pthread.h>
#include <stdlib.h> // For atoi
#include <getopt.h> // For getopt


#define NUM_THREADS 10


struct thread_data
{
    int *sharedVariable;
    pthread_mutex_t *mutex;
    int useMutex;
};


static void usage(const char *program_name, int exit_code, const char *message);
static void *threadFunction(void *arg);


int main(int argc, char *argv[])
{
    int opt;
    int useMutex = 0; // Default is no mutex

    // Process command-line options
    while((opt = getopt(argc, argv, "hm")) != -1)
    {
        switch(opt)
        {
            case 'h':
            {
                usage(argv[0], EXIT_SUCCESS, NULL);
                return EXIT_SUCCESS;
            }
            case 'm':
            {
                useMutex = 1;
                break;
            }
            case '?':
            {
                char message[24];

                snprintf(message, sizeof(message), "Unknown option '-%c'.\n", optopt);
                usage(argv[0], EXIT_FAILURE, message);
                break;
            }
            default:
            {
                usage(argv[0], EXIT_FAILURE, NULL);
            }
        }
    }

    // Initialize the mutex if needed
    pthread_mutex_t mutex;

    if(useMutex && pthread_mutex_init(&mutex, NULL) != 0)
    {
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
        if(pthread_create(&threads[i], NULL, threadFunction, (void *) &data) != 0)
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
    if(useMutex)
    {
        pthread_mutex_destroy(&mutex);
    }

    return EXIT_SUCCESS;
}


static void *threadFunction(void *arg)
{
    struct thread_data *data = (struct thread_data *) arg;

    if(data->useMutex)
    {
        // Lock the mutex before accessing the shared variable
        pthread_mutex_lock(data->mutex);
    }

    // Critical section: Accessing and modifying the shared variable
    (*(data->sharedVariable))++;
    printf("Thread %ld: Shared variable value: %d\n", (long) pthread_self(), *(data->sharedVariable));
    // sleep(1); // Introduce a 1-second delay

    if(data->useMutex)
    {
        // Unlock the mutex after finishing the critical section
        pthread_mutex_unlock(data->mutex);
    }

    // Exit the thread
    pthread_exit(NULL);
}


static void usage(const char *program_name, int exit_code, const char *message)
{
    fprintf(stderr, "Usage: ./program_name [-h] [-m]\n");
    fputs("Options:\n", stderr);
    fputs("  -h        Display this help message.\n", stderr);
    fputs("  -m        Use mutex for threadFunction (default is no mutex).\n", stderr);
}

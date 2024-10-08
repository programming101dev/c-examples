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

#include <getopt.h>
#include <inttypes.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void           parse_arguments(int argc, char *argv[], bool *use_mutex);
_Noreturn static void usage(const char *program_name, int exit_code, const char *message);
static void          *thread_function(void *arg);

#define NUM_THREADS 10
#define UNKNOWN_OPTION_MESSAGE_LEN 24

struct thread_data
{
    bool             use_mutex;
    pthread_mutex_t *mutex;
    int             *sharedVariable;
};

int main(int argc, char *argv[])
{
    bool               use_mutex;
    pthread_mutex_t    mutex;
    pthread_t          threads[NUM_THREADS];
    int                sharedVariable;
    int                i;
    struct thread_data data;

    use_mutex = false;
    parse_arguments(argc, argv, &use_mutex);

    if(use_mutex && pthread_mutex_init(&mutex, NULL) != 0)
    {
        fprintf(stderr, "Error: Mutex initialization failed.\n");
        return EXIT_FAILURE;
    }

    sharedVariable = 0;

    // Create multiple threads
    data.sharedVariable = &sharedVariable;
    data.mutex          = &mutex;
    data.use_mutex      = use_mutex;

    for(i = 0; i < NUM_THREADS; i++)
    {
        if(pthread_create(&threads[i], NULL, thread_function, (void *)&data) != 0)
        {
            fprintf(stderr, "Error: Thread creation failed.\n");
            return EXIT_FAILURE;
        }
    }

    // Wait for all threads to finish
    for(i = 0; i < NUM_THREADS; i++)
    {
        pthread_join(threads[i], NULL);
    }

    // Destroy the mutex if needed
    if(use_mutex)
    {
        pthread_mutex_destroy(&mutex);
    }

    return EXIT_SUCCESS;
}

static void parse_arguments(int argc, char *argv[], bool *use_mutex)
{
    int opt;

    opterr = 0;

    while((opt = getopt(argc, argv, "hm")) != -1)
    {
        switch(opt)
        {
            case 'm':
            {
                *use_mutex = 1;
                break;
            }
            case 'h':
            {
                usage(argv[0], EXIT_SUCCESS, NULL);
            }
            case '?':
            {
                char message[UNKNOWN_OPTION_MESSAGE_LEN];

                snprintf(message, sizeof(message), "Unknown option '-%c'.", optopt);
                usage(argv[0], EXIT_FAILURE, message);
            }
            default:
            {
                usage(argv[0], EXIT_FAILURE, NULL);
            }
        }
    }
}

_Noreturn static void usage(const char *program_name, int exit_code, const char *message)
{
    if(message)
    {
        fprintf(stderr, "%s\n", message);
    }

    fprintf(stderr, "Usage: %s [-h] [-m]\n", program_name);
    fputs("Options:\n", stderr);
    fputs("  -h  Display this help message.\n", stderr);
    fputs("  -m  Use mutex (default is no mutex).\n", stderr);
    exit(exit_code);
}

#if defined(__clang__)
    #pragma clang diagnostic push
    #pragma clang diagnostic ignored "-Wthread-safety-analysis"
#endif
static void *thread_function(void *arg)
{
    pthread_t           tid;
    struct thread_data *data;

    data = (struct thread_data *)arg;

    if(data->use_mutex)
    {
        // Lock the mutex before accessing the shared variable
        pthread_mutex_lock(data->mutex);
    }

    // Critical section: Accessing and modifying the shared variable
    (*(data->sharedVariable))++;

    // Get the thread ID
    tid = pthread_self();

    // Print the thread ID and shared variable value (using pointer representation for pthread_t)
    printf("Thread %lu: Shared variable value: %d\n", (unsigned long)tid, *(data->sharedVariable));

    if(data->use_mutex)
    {
        // Unlock the mutex after finishing the critical section
        pthread_mutex_unlock(data->mutex);
    }

    pthread_exit(NULL);
}
#if defined(__clang__)
    #pragma clang diagnostic pop
#endif

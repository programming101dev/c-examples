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

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

static void *thread_function(void *arg);

int main(int argc, char *argv[])
{
    pthread_t thread1;
    pthread_t thread2;

    pthread_create(&thread1, NULL, thread_function, NULL);
    pthread_create(&thread2, NULL, thread_function, NULL);

    // Check if thread1 and thread1 have the same thread ID
    if(pthread_equal(thread1, thread1))
    {
        printf("Thread 1 and Thread 1 have the same thread ID.\n");
    }
    else
    {
        printf("Thread 1 and Thread 1 have different thread IDs.\n");
    }

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

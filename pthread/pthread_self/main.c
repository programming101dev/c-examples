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
    pthread_t thread_id;

    thread_id = pthread_self();
    printf("Thread ID: %lu\n", (unsigned long)thread_id);

    // Perform some work in the thread...

    return NULL;
}

#pragma GCC diagnostic pop

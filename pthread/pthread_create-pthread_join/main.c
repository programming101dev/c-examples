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
#include <unistd.h>

static void *print_numbers(void *arg);

// TODO read this in on the command line
#define NUMBERS_LEN 5

int main(void)
{
    pthread_t thread_id;
    int       result;

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

// cppcheck-suppress constParameterCallback
static void *print_numbers(void *arg)
{
    // TODO pass the NUMBERS_LEN in the arg variable
    for(int i = 1; i <= NUMBERS_LEN; ++i)
    {
        printf("Thread: %d\n", i);
        sleep(1);
    }

    // The thread returns NULL upon completion
    return NULL;
}

#pragma GCC diagnostic pop

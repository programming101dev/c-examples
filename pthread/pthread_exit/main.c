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
#include <stdlib.h>
#include <pthread.h>


static void *thread_function(void *arg);


int main(void)
{
    pthread_t threads[3];
    int       thread_args[3] = {
            1,
            2,
            3
    };
    for(int   i              = 0; i < 3; i++)
    {
        pthread_create(&threads[i], NULL, thread_function, &thread_args[i]);
    }

    // Wait for all threads to finish and retrieve their exit values
    for(int   i              = 0; i < 3; i++)
    {
        int *result;
        pthread_join(threads[i], (void **)&result);
        printf("Thread %d returned: %d\n", i + 1, *result);
        free(result); // Don't forget to free the memory returned by the thread
    }
    printf("Main thread is done.\n");
    return EXIT_SUCCESS;
}


static void *thread_function(void *arg)
{
    int thread_id;
    int *result;
    thread_id = *(int *)arg;
    printf("Thread %d is executing.\n", thread_id);

    // Perform some work in the thread...

    printf("Thread %d is done.\n", thread_id);

    // Exit the thread and return a value (optional)
    result = (int *)malloc(sizeof(int));
    if(result == NULL)
    {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    *result = thread_id * 2;
    pthread_exit(result);
}

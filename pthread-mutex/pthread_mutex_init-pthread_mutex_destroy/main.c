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

int main(void)
{
    pthread_mutex_t mutex;

    if(pthread_mutex_init(&mutex, NULL) != 0)
    {
        fprintf(stderr, "Error: Mutex initialization failed.\n");
        return EXIT_FAILURE;
    }

    printf("mutex created\n");
    pthread_mutex_destroy(&mutex);
    printf("mutex destroyed\n");

    return EXIT_SUCCESS;
}

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


int main(void)
{
    pthread_cond_t condition;
    int            destroy_result;

    // Initialize the condition variable
    int            init_result = pthread_cond_init(&condition, NULL);
    if(init_result != 0)
    {
        fprintf(stderr, "Error initializing condition variable. Error code: %d\n", init_result);
        return EXIT_FAILURE;
    }
    printf("condition created\n");

    // Destroy the condition variable
    destroy_result = pthread_cond_destroy(&condition);
    if(destroy_result != 0)
    {
        fprintf(stderr, "Error destroying condition variable. Error code: %d\n", destroy_result);
        return EXIT_FAILURE;
    }
    printf("condition destroyed\n");
    return EXIT_SUCCESS;
}

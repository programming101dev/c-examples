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


#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>


int main(void)
{
    struct timespec tim;
    struct timespec rem;

    tim.tv_sec  = 1; // seconds
    tim.tv_nsec = 500000000; // 0.5 seconds in nanoseconds
    printf("Starting program...\n");
    printf("Sleeping for 1.5 seconds...\n");

    while(nanosleep(&tim, &rem) == -1)
    {
        if(errno == EINTR)
        {
            tim = rem; // If interrupted by a signal, assign the remaining time to tim
            continue;
        }
        else
        {
            perror("Unexpected error from nanosleep");
            return EXIT_FAILURE;
        }
    }

    printf("Program woke up!\n");

    return EXIT_SUCCESS;
}

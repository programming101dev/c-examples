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
#include <time.h>
#include <unistd.h>


// TODO: get time to sleep from the command line


int main(void)
{
    time_t start_time;
    time_t end_time;
    double diff_time;

    // Get the start time
    if(time(&start_time) == -1)
    {
        perror("Error getting start time");
        return EXIT_FAILURE;
    }

    // Simulate some time passing
    sleep(5); // Sleep for 5 seconds

    // Get the end time
    if(time(&end_time) == -1)
    {
        perror("Error getting end time");
        return EXIT_FAILURE;
    }

    // Calculate the time difference
    if(end_time == (time_t)-1 || start_time == (time_t)-1)
    {
        perror("Invalid time values");
        return EXIT_FAILURE;
    }

    diff_time = difftime(end_time, start_time);
    printf("Time difference: %.2f seconds\n", diff_time);

    return EXIT_SUCCESS;
}

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


#define BUFFER_LEN 80


int main(void)
{
    time_t    rawtime;
    struct tm *timeinfo;
    char      buffer[BUFFER_LEN];

    // Get the current time
    if(time(&rawtime) == -1)
    {
        perror("Error getting current time");
        return EXIT_FAILURE;
    }

    // Convert the time to a local time representation
    timeinfo = gmtime(&rawtime);

    if(timeinfo == NULL)
    {
        perror("Error converting to GMT/UTC time");
        return EXIT_FAILURE;
    }

    // Format the time
    if(strftime(buffer, sizeof(buffer), "GMT/UTC Time: %Y-%m-%d %H:%M:%S\n", timeinfo) == 0)
    {
        perror("Error formatting time");
        return EXIT_FAILURE;
    }

    printf("%s", buffer);

    return EXIT_SUCCESS;
}

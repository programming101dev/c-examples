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
#include <unistd.h>

#define PATH_LEN 4096    // A common default value for the maximum path length

// TODO pass the filename in on the command line

int main(void)
{
    const char *path = ".";
    long        path_max;
    char       *buffer;
    size_t      size;

    path_max = pathconf(path, _PC_PATH_MAX);

    if(path_max == -1)
    {
        // Error occurred
        path_max = PATH_LEN;
    }

    buffer = NULL;
    size   = (size_t)path_max;

    while(1)
    {
        char *temp_buffer;

        // Allocate memory for the buffer (or resize the existing buffer)
        temp_buffer = (char *)realloc(buffer, size);

        if(temp_buffer == NULL)
        {
            perror("Error allocating/reallocating memory for buffer");
            free(buffer);
            return EXIT_FAILURE;
        }

        buffer = temp_buffer;

        if(getcwd(buffer, size) != NULL)
        {
            break;
        }

        // Check if the failure was due to insufficient buffer size
        if(errno == ERANGE)
        {
            // Retry with a larger buffer size
            size *= 2;    // Double the buffer size
        }
        else
        {
            perror("Error getting current working directory");
            free(buffer);
            return EXIT_FAILURE;
        }
    }

    printf("Current working directory: %s\n", buffer);
    free(buffer);

    return EXIT_SUCCESS;
}

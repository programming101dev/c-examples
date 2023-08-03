#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

int main(void)
{
    const char *path = ".";
    long path_max;
    char *buffer = NULL;
    size_t size = path_max;

    path_max = pathconf(path, _PC_PATH_MAX);

    if(path_max == -1)
    {
        // Error occurred
        path_max = 4096; // A common default value for the maximum path length
    }

    buffer = NULL;
    size   = path_max;

    if(chdir("/") == -1)
    {
        perror("chdir");
        return 1;
    }

    while(1)
    {
        // Allocate memory for the buffer (or resize the existing buffer)
        buffer = realloc(buffer, size);

        if(buffer == NULL)
        {
            perror("Error allocating/reallocating memory for buffer");
            return EXIT_FAILURE;
        }

        if(getcwd(buffer, size) != NULL)
        {
            printf("Current working directory: %s\n", buffer);
            free(buffer);
            return EXIT_SUCCESS;
        }
        else
        {
            // Check if the failure was due to insufficient buffer size
            if(errno == ERANGE)
            {
                // Retry with a larger buffer size
                size *= 2; // Double the buffer size
            }
            else
            {
                perror("Error getting current working directory");
                free(buffer);
                return EXIT_FAILURE;
            }
        }
    }

    return EXIT_SUCCESS;
}

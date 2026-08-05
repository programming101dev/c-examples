/*
 * Copyright 2026 D'Arcy Smith
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define PATH_LEN 4096    // A common default value for the maximum path length

int main(void)
{
    const char *path = ".";
    long        path_max;
    char       *buffer;
    size_t      size;

    path_max = pathconf(path, _PC_PATH_MAX);

    if(path_max == -1)
    {
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

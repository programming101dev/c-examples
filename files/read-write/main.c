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

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define BUFFER_SIZE 4096

int main(void)
{
    char    buffer[BUFFER_SIZE];
    ssize_t bytesRead;

    printf("Press ctrl-d to exit.\n");
    printf("Start typing...\n");
    fflush(stdout);

    while((bytesRead = read(STDIN_FILENO, buffer, sizeof(buffer))) > 0)
    {
        size_t  bytes_to_write;
        ssize_t bytesWritten;

        bytes_to_write = (size_t)bytesRead;
        bytesWritten   = write(STDOUT_FILENO, buffer, bytes_to_write);

        if(bytesWritten != bytesRead)
        {
            perror("Error writing to stdout");
            return EXIT_FAILURE;
        }
    }

    if(bytesRead == -1)
    {
        perror("Error reading from stdin");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

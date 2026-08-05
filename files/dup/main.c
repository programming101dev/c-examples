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

int main(void)
{
    int stdout_dup_fd;

    stdout_dup_fd = dup(STDOUT_FILENO);    // NOLINT(android-cloexec-dup)

    if(stdout_dup_fd == -1)
    {
        perror("Error duplicating stdout");
        return EXIT_FAILURE;
    }

    // Close the original stdout file descriptor (STDOUT_FILENO)
    if(close(STDOUT_FILENO) == -1)
    {
        perror("Error closing stdout");
        close(stdout_dup_fd);    // Close the duplicated file descriptor before exiting
        return EXIT_FAILURE;
    }

    // Use the duplicated file descriptor to write to stdout using dprintf
    if(dprintf(stdout_dup_fd, "This will be written using dprintf to the duplicated file descriptor.\n") == -1)
    {
        perror("Error writing to stdout using dprintf");
        close(stdout_dup_fd);    // Close the duplicated file descriptor before exiting
        return EXIT_FAILURE;
    }

    // Close the duplicated file descriptor
    if(close(stdout_dup_fd) == -1)
    {
        perror("Error closing duplicated file descriptor");
        return EXIT_FAILURE;
    }

    // The standard output (stdout) is now closed, so printf will not work
    printf("This will not be printed because stdout is closed.\n");

    return EXIT_SUCCESS;
}

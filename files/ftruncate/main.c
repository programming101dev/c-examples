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
#include <sys/stat.h>
#include <unistd.h>

static void check_file_size(const char *filename, const char *message);

// TODO - pass the size in on the command line
#define SIZE 9

int main(void)
{
    // TODO pass the tempate and content in on the command line
    const char *content    = "This is a sample file.\n";
    char        filename[] = "example_XXXXXX";
    int         fd;
    off_t       new_size;

    fd = mkstemp(filename);

    if(fd == -1)
    {
        perror("Error creating temporary file");
        return EXIT_FAILURE;
    }

    dprintf(fd, "%s\n", content);
    check_file_size(filename, "Initial file size");
    new_size = SIZE;
    printf("Truncating file %s to %lld bytes\n", filename, (long long)new_size);

    if(ftruncate(fd, new_size) == -1)
    {
        perror("Error truncating file");
        close(fd);
        return EXIT_FAILURE;
    }

    if(close(fd) == -1)
    {
        perror("Error closing file");
        return EXIT_FAILURE;
    }

    if(unlink(filename) == -1)
    {
        perror("Error unlinking file");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

static void check_file_size(const char *filename, const char *message)
{
    struct stat st;

    if(stat(filename, &st) == 0)
    {
        printf("%s of '%s' is %lld bytes.\n", message, filename, (long long)st.st_size);
        printf("%s\n", message);
    }
    else
    {
        perror("Error getting file size");
    }
}

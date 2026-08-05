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
    // TODO pass the filename in on the command line
    const char *filename = "example.txt";
    off_t       new_size;
    FILE       *file;

    file = fopen(filename, "we");

    if(file != NULL)
    {
        fprintf(file, "This is a sample file.\n");
        fclose(file);
    }

    check_file_size(filename, "Truncated file size");
    new_size = SIZE;
    printf("Truncating file %s to %lld bytes\n", filename, (long long)new_size);

    if(truncate(filename, new_size) == -1)
    {
        perror("Error truncating file");
        return EXIT_FAILURE;
    }

    check_file_size(filename, "Truncated file size");

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
        printf("\t%s of '%s' is %lld bytes.\n", message, filename, (long long)st.st_size);
    }
    else
    {
        perror("Error getting file size");
    }
}

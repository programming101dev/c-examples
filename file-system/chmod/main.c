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

static void print_permissions(const char *filename);

// TODO pass the filename in on the command line

int main(void)
{
    const char *filename = "file.txt";
    FILE       *file;
    mode_t      new_permissions;

    file = fopen(filename, "we");

    if(file == NULL)
    {
        perror("Error creating file");
        return EXIT_FAILURE;
    }

    fclose(file);
    printf("Before chmod:\n");
    print_permissions(filename);
    new_permissions = S_IRUSR | S_IWUSR | S_IXUSR | S_IWGRP | S_IRGRP | S_IROTH;    // rwx-w-r--

    if(chmod(filename, new_permissions) == -1)
    {
        perror("Error changing file permissions");
        return EXIT_FAILURE;
    }

    printf("\nFile permissions changed successfully.\n");
    printf("\nAfter chmod:\n");
    print_permissions(filename);

    if(unlink(filename) == -1)
    {
        perror("Error removing file");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

static void print_permissions(const char *filename)
{
    struct stat file_stat;

    if(stat(filename, &file_stat) == -1)
    {
        perror("Error getting file stats");
        exit(EXIT_FAILURE);
    }

    printf("File: %s\n", filename);
    printf("Permissions: %o\n", (unsigned int)file_stat.st_mode & (S_IRWXU | S_IRWXG | S_IRWXO));
}

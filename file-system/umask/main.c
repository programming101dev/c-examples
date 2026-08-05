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

static void create_file(const char *filename, const char *content);
static void print_file_permissions(const char *filename);
static void delete_file(const char *filename);

int main(void)
{
    // TODO pass the filenames and content in on the command line
    const char *filename1 = "file1.txt";
    const char *filename2 = "file2.txt";
    const char *content   = "This is a sample file.\n";
    mode_t      old_mask;

    old_mask = umask(0);
    create_file(filename1, content);
    printf("File created with umask 000:\n");
    print_file_permissions(filename1);
    umask(S_IWUSR | S_IWGRP | S_IWOTH);
    create_file(filename2, content);
    printf("\nFile created with umask 222:\n");
    print_file_permissions(filename2);
    umask(old_mask);
    delete_file(filename1);
    delete_file(filename2);

    return EXIT_SUCCESS;
}

static void create_file(const char *filename, const char *content)
{
    FILE *file;

    file = fopen(filename, "we");

    if(file == NULL)
    {
        perror("Error creating file");
        exit(EXIT_FAILURE);
    }

    if(fprintf(file, "%s", content) < 0)
    {
        perror("Error writing to file");
        fclose(file);
        exit(EXIT_FAILURE);
    }

    fclose(file);
}

static void print_file_permissions(const char *filename)
{
    struct stat fileStat;

    if(stat(filename, &fileStat) == -1)
    {
        perror("Error getting file stats");
        return;
    }

    printf("File: %s\n", filename);
    printf("Permissions: %o\n", (unsigned int)fileStat.st_mode & (S_IRWXU | S_IRWXG | S_IRWXO));
}

static void delete_file(const char *filename)
{
    if(unlink(filename) == -1)
    {
        perror("Error deleting file");
        exit(EXIT_FAILURE);
    }
}

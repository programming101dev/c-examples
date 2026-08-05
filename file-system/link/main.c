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

static void create_sample_file(const char *filename);
static void create_hard_link(const char *source_filename, const char *link_filename);
static void check_file_existence(const char *filename, const char *message);

int main(void)
{
    // TODO pass the filenames in on the command line
    const char *source_filename = "example.txt";
    const char *link_filename   = "hard_link.txt";

    printf("Creating file %s\n", source_filename);
    create_sample_file(source_filename);
    check_file_existence(source_filename, "Source file");
    check_file_existence(link_filename, "Link file");
    printf("Creating hard link %s\n", link_filename);
    create_hard_link(source_filename, link_filename);
    check_file_existence(source_filename, "Source file");
    check_file_existence(link_filename, "Hard link");
    printf("Deleting file %s\n", source_filename);

    if(unlink(source_filename) == -1)
    {
        perror("Error deleting source file");
        return EXIT_FAILURE;
    }

    check_file_existence(source_filename, "Source file");
    check_file_existence(link_filename, "Hard link");
    printf("Deleting hard link %s\n", link_filename);

    if(unlink(link_filename) == -1)
    {
        perror("Error deleting hard link");
        return EXIT_FAILURE;
    }

    check_file_existence(source_filename, "Source file");
    check_file_existence(link_filename, "Hard link");

    return EXIT_SUCCESS;
}

static void create_sample_file(const char *filename)
{
    FILE *file;

    file = fopen(filename, "we");

    if(file != NULL)
    {
        fprintf(file, "This is a sample file.\n");
        fclose(file);
    }
}

static void create_hard_link(const char *source_filename, const char *link_filename)
{
    if(link(source_filename, link_filename) == -1)
    {
        perror("Error creating hard link");
        exit(EXIT_FAILURE);
    }
}

static void check_file_existence(const char *filename, const char *message)
{
    if(access(filename, F_OK) == 0)
    {
        printf("\t%s '%s' exists.\n", message, filename);
    }
    else
    {
        printf("\t%s '%s' does not exist.\n", message, filename);
    }
}

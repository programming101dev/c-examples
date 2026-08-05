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

static void check_file_existence(const char *filename, const char *message);

int main(void)
{
    // TODO pass the filenames in on the command line
    const char *old_filename = "old_file.txt";
    const char *new_filename = "new_file.txt";
    FILE       *file;

    file = fopen(old_filename, "we");

    if(file != NULL)
    {
        fprintf(file, "This is a sample file.\n");
        fclose(file);
    }

    printf("Creating file %s\n", old_filename);
    check_file_existence(old_filename, "Original file");
    check_file_existence(new_filename, "Renamed file");
    printf("Renaming file %s to %s\n", old_filename, new_filename);

    if(rename(old_filename, new_filename) == -1)
    {
        perror("Error renaming file");
        return EXIT_FAILURE;
    }

    check_file_existence(old_filename, "Original file");
    check_file_existence(new_filename, "Renamed file");
    printf("Removing file %s\n", new_filename);

    if(unlink(new_filename) == -1)
    {
        perror("Error unlinking file");
        return EXIT_FAILURE;
    }

    check_file_existence(old_filename, "Original file");
    check_file_existence(new_filename, "Renamed file");

    return EXIT_SUCCESS;
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

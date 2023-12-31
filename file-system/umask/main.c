/*
 * This code is licensed under the Attribution-NonCommercial-NoDerivatives 4.0 International license.
 *
 * Author: D'Arcy Smith (ds@programming101.dev)
 *
 * You are free to:
 *   - Share: Copy and redistribute the material in any medium or format.
 *   - Under the following terms:
 *       - Attribution: You must give appropriate credit, provide a link to the license, and indicate if changes were made.
 *       - NonCommercial: You may not use the material for commercial purposes.
 *       - NoDerivatives: If you remix, transform, or build upon the material, you may not distribute the modified material.
 *
 * For more details, please refer to the full license text at:
 * https://creativecommons.org/licenses/by-nc-nd/4.0/
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

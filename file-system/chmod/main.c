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


void print_permissions(const char *filename);


int main(void)
{
    // TODO pass the filename in on the command line
    const char *filename = "file.txt";
    mode_t     new_permissions;
    FILE       *file;

    file = fopen(filename, "we");

    if(file == NULL)
    {
        perror("Error creating file");
        return EXIT_FAILURE;
    }

    fclose(file);
    printf("Before chmod:\n");
    print_permissions(filename);
    new_permissions = S_IRUSR | S_IWUSR | S_IXUSR | S_IWGRP | S_IRGRP | S_IROTH; // rwx-w-r--

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


void print_permissions(const char *filename)
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

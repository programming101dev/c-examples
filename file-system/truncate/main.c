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

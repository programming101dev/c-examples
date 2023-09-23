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


int main(void)
{
    const char *content   = "This is a sample file.\n";
    char       filename[] = "example_XXXXXX";
    int        fd;
    off_t      new_size;

    fd = mkstemp(filename);

    if(fd == -1)
    {
        perror("Error creating temporary file");
        return EXIT_FAILURE;
    }

    dprintf(fd, "%s\n", content);
    check_file_size(filename, "Initial file size");
    new_size = 9;
// XXXXXX    printf("Truncating file %s to %lld bytes\n", filename, (long long) new_size);

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

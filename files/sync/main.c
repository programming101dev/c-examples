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
#include <unistd.h>


int main(void)
{
    char       templ[] = "/tmp/mytempfileXXXXXX";
    const char *data   = "This is some data that needs to be written to the file.\n";
    int        fd;

    printf("Creating a temporary file...\n");
    fd = mkstemp(templ);

    if(fd == -1)
    {
        perror("Error creating temporary file");
        return EXIT_FAILURE;
    }

    if(dprintf(fd, "%s", data) < 0)
    {
        perror("Error writing to temporary file");
        close(fd);
        return EXIT_FAILURE;
    }

    printf("Flushing data to disk...\n");
    sync();
    printf("Data successfully flushed to disk.\n");

    if(close(fd) == -1)
    {
        perror("Error closing file descriptor");
        return EXIT_FAILURE;
    }

    printf("Deleting the temporary file...\n");

    if(unlink(templ) == -1)
    {
        perror("Error deleting temporary file");
        return EXIT_FAILURE;
    }

    printf("Temporary file deleted successfully.\n");

    return EXIT_SUCCESS;
}

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
    // TODO pass on the command line
    char templ[] = "/tmp/temp_file_XXXXXX";
    int  fd;

    // Create a temporary file using mkstemp
    fd = mkstemp(templ);

    if(fd == -1)
    {
        perror("Error creating temporary file");
        return EXIT_FAILURE;
    }

    printf("Created temporary file %s\n", templ);
    close(fd);

    // Delete the temporary file using unlink
    if(unlink(templ) == -1)
    {
        perror("Error deleting temporary file");
        return EXIT_FAILURE;
    }

    printf("Temporary file deleted successfully.\n");

    return EXIT_SUCCESS;
}

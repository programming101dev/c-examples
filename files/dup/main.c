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
#include <unistd.h>
#include <stdlib.h>


int main(void)
{
    int stdout_dup_fd = dup(STDOUT_FILENO);
    if(stdout_dup_fd == -1)
    {
        perror("Error duplicating stdout");
        return EXIT_FAILURE;
    }

    // Close the original stdout file descriptor (STDOUT_FILENO)
    if(close(STDOUT_FILENO) == -1)
    {
        perror("Error closing stdout");
        close(stdout_dup_fd); // Close the duplicated file descriptor before exiting
        return EXIT_FAILURE;
    }

    // Use the duplicated file descriptor to write to stdout using dprintf
    if(dprintf(stdout_dup_fd, "This will be written using dprintf to the duplicated file descriptor.\n") == -1)
    {
        perror("Error writing to stdout using dprintf");
        close(stdout_dup_fd); // Close the duplicated file descriptor before exiting
        return EXIT_FAILURE;
    }

    // Close the duplicated file descriptor
    if(close(stdout_dup_fd) == -1)
    {
        perror("Error closing duplicated file descriptor");
        return EXIT_FAILURE;
    }

    // The standard output (stdout) is now closed, so printf will not work
    printf("This will not be printed because stdout is closed.\n");
    return EXIT_SUCCESS;
}

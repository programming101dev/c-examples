#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>


int main(void)
{
    int stdout_dup_fd = dup(STDOUT_FILENO);

    if (stdout_dup_fd == -1)
    {
        perror("Error duplicating stdout");
        return EXIT_FAILURE;
    }

    // Close the original stdout file descriptor (STDOUT_FILENO)
    if (close(STDOUT_FILENO) == -1)
    {
        perror("Error closing stdout");
        close(stdout_dup_fd); // Close the duplicated file descriptor before exiting
        return EXIT_FAILURE;
    }

    // Use the duplicated file descriptor to write to stdout using dprintf
    if (dprintf(stdout_dup_fd, "This will be written using dprintf to the duplicated file descriptor.\n") == -1)
    {
        perror("Error writing to stdout using dprintf");
        close(stdout_dup_fd); // Close the duplicated file descriptor before exiting
        return EXIT_FAILURE;
    }

    // Close the duplicated file descriptor
    if (close(stdout_dup_fd) == -1)
    {
        perror("Error closing duplicated file descriptor");
        return EXIT_FAILURE;
    }

    // The standard output (stdout) is now closed, so printf will not work
    printf("This will not be printed because stdout is closed.\n");

    return EXIT_SUCCESS;
}

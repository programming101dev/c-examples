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
#include <sys/stat.h>


int main(void)
{
    char template[] = "/tmp/tempfileXXXXXX";
    int original_fd;
    struct stat file_stat;

    // Create a temporary file and obtain a unique filename
    original_fd = mkstemp(template);

    if(original_fd == -1)
    {
        perror("Error creating temporary file");
        return EXIT_FAILURE;
    }

    // Get the size of the temporary file before writing
    if(stat(template, &file_stat) == -1)
    {
        perror("Error getting file stats");
        close(original_fd); // Close the file descriptor before exiting
        return EXIT_FAILURE;
    }

    fprintf(stderr, "Size of the temporary file before writing: %lld bytes\n", (long long) file_stat.st_size);

    // Redirect stdout (file descriptor 1) to the temporary file
    if(dup2(original_fd, STDOUT_FILENO) == -1)
    {
        perror("Error redirecting stdout");
        close(original_fd); // Close the file descriptor before exiting
        return EXIT_FAILURE;
    }

    // Now, printf will write to the temporary file
    if(printf("This will be written to the temporary file using printf.\n") < 0)
    {
        perror("Error writing to temporary file");
        close(original_fd);
        return EXIT_FAILURE;
    }

    // Flush the output buffer to ensure data is written to the file
    if(fflush(stdout) != 0)
    {
        perror("Error flushing stdout");
        close(original_fd); // Close the file descriptor before exiting
        return EXIT_FAILURE;
    }

    // Close the temporary file descriptor
    if(close(original_fd) == -1)
    {
        perror("Error closing temporary file descriptor");
        return EXIT_FAILURE;
    }

    // Get the size of the temporary file after writing
    if(stat(template, &file_stat) == -1)
    {
        perror("Error getting file stats");
        return EXIT_FAILURE;
    }

    fprintf(stderr, "Size of the temporary file after writing: %lld bytes\n", (long long) file_stat.st_size);

    // Cleanup: remove the temporary file
    if(unlink(template) == -1)
    {
        perror("Error removing temporary file");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

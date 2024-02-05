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

#if defined(__GNUC__) && !defined(__clang__) && !defined(__FreeBSD__)
    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wanalyzer-fd-leak"
#endif
int main(void)
{
    char        templ[] = "/tmp/tempfileXXXXXX";
    int         original_fd;
    struct stat file_stat;
    int         saved_stdout;

    original_fd = mkstemp(templ);
    if(original_fd == -1)
    {
        perror("Error creating temporary file");
        return EXIT_FAILURE;
    }

    if(stat(templ, &file_stat) == -1)
    {
        perror("Error getting file stats");
        close(original_fd);
        return EXIT_FAILURE;
    }

    fprintf(stderr, "Size of the temporary file before writing: %lld bytes\n", (long long)file_stat.st_size);
    saved_stdout = dup(STDOUT_FILENO);    // NOLINT(android-cloexec-dup)

    if(saved_stdout == -1)
    {
        perror("Error saving STDOUT");
        close(original_fd);
        return EXIT_FAILURE;
    }

    if(dup2(original_fd, STDOUT_FILENO) == -1)
    {
        perror("Error redirecting stdout");
        close(original_fd);
        close(saved_stdout);
        return EXIT_FAILURE;
    }

    if(printf("This will be written to the temporary file using printf.\n") < 0)
    {
        perror("Error writing to temporary file");
        close(original_fd);
        dup2(saved_stdout, STDOUT_FILENO);
        close(saved_stdout);
        return EXIT_FAILURE;
    }

    if(fflush(stdout) != 0)
    {
        perror("Error flushing stdout");
        close(original_fd);
        dup2(saved_stdout, STDOUT_FILENO);
        close(saved_stdout);
        return EXIT_FAILURE;
    }

    dup2(saved_stdout, STDOUT_FILENO);
    close(saved_stdout);

    if(close(original_fd) == -1)
    {
        perror("Error closing temporary file descriptor");
        return EXIT_FAILURE;
    }

    if(stat(templ, &file_stat) == -1)
    {
        perror("Error getting file stats");
        return EXIT_FAILURE;
    }

    fprintf(stderr, "Size of the temporary file after writing: %lld bytes\n", (long long)file_stat.st_size);

    if(unlink(templ) == -1)
    {
        perror("Error removing temporary file");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

#if defined(__GNUC__) && !defined(__clang__) && !defined(__FreeBSD__)
    #pragma GCC diagnostic pop
#endif

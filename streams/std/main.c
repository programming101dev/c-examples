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

int main(void)
{
    int stdin_fd;
    int stdout_fd;
    int stderr_fd;

    stdin_fd  = fileno(stdin);
    stdout_fd = fileno(stdout);
    stderr_fd = fileno(stderr);
    printf("File descriptor for stdin: %d\n", stdin_fd);
    printf("File descriptor for stdout: %d\n", stdout_fd);
    printf("File descriptor for stderr: %d\n", stderr_fd);

    return EXIT_SUCCESS;
}

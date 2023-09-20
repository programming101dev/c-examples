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
    pid_t pid;

    pid = getpid();
    printf("Current Process ID: %d\n", pid);

    // Execute the "ls -l" command using execl
    // The first argument is the path to the command (in this case, "ls")
    // The subsequent arguments are the command-line arguments to "ls" (in this case, "-l" for long format listing)
    if(execl("/bin/ls", "ls", "-l", (char *) NULL) == -1)
    {
        perror("execl");
        exit(EXIT_FAILURE);
    }

    // The code below this line will not be executed, as the process image is replaced by the new program.
    // However, it's a good practice to include it for completeness.

    printf("This line will not be executed if execl is successful.\n");

    return EXIT_SUCCESS;
}

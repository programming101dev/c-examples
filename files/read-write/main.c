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


#define BUFFER_SIZE 4096


int main(void)
{
    char buffer[BUFFER_SIZE];
    ssize_t bytesRead;

    printf("Press ctrl-d to exit.\n");
    printf("Start typing...\n");
    fflush(stdout);

    while((bytesRead = read(STDIN_FILENO, buffer, sizeof(buffer))) > 0)
    {
        ssize_t bytesWritten;

        bytesWritten = write(STDOUT_FILENO, buffer, bytesRead);

        if(bytesWritten != bytesRead)
        {
            perror("Error writing to stdout");
            return EXIT_FAILURE;
        }
    }

    if(bytesRead == -1)
    {
        perror("Error reading from stdin");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

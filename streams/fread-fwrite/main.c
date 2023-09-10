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


#define BUFFER_SIZE 256


int main(void)
{
    char buffer[BUFFER_SIZE];
    size_t bytesRead;

    buffer[0] = '\0';
    printf("Enter some text (press Ctrl+D to stop):\n");

    // Read up to BUFFER_SIZE - 1 bytes from stdin, leaving room for a null terminator
    // NOTE: it will not return when \n is presed, you have to reach the size or end of file
    //       before fread returns
    while((bytesRead = fread(buffer, 1, BUFFER_SIZE - 1, stdin)) > 0)
    {
        // Add a null terminator to the end of the buffer
        buffer[bytesRead] = '\0';

        // Print the buffer
        fputs("You typed: ", stdout);
        fwrite(buffer, 1, bytesRead, stdout);

        // Clear the buffer
        buffer[0] = '\0';
    }

    return EXIT_SUCCESS;
}

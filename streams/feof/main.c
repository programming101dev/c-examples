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

    printf("Enter some text (press Ctrl+D to stop):\n");

    while(!feof(stdin))
    {
        if(fgets(buffer, BUFFER_SIZE, stdin) != NULL)
        {
            printf("You entered: %s", buffer);
        }
    }

    printf("\nReached the end of input.\n");

    return EXIT_SUCCESS;
}

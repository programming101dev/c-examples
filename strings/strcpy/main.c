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
#include <string.h>


int main(void)
{
    const char *source = "Hello, World!";
    size_t destination_size;
    char *destination;

    destination_size = strlen(source) + 1;
    destination = malloc(destination_size);

    if(destination == NULL)
    {
        perror("Memory allocation failed\n");

        return EXIT_FAILURE;
    }

    strcpy(destination, source);
    printf("Copied string: %s\n", destination);
    free(destination);

    return EXIT_SUCCESS;
}

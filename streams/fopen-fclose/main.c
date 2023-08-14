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


#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int main(int argc, char *argv[])
{
    const char *filename;
    FILE *file;

    if(argc != 2)
    {
        fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
        return EXIT_FAILURE;
    }

    filename = argv[1];
    file = fopen(filename, "rb");

    if(file == NULL)
    {
        fprintf(stderr, "Error opening the file '%s': %s\n", filename, strerror(errno));
        return EXIT_FAILURE;
    }

    printf("Successfully opened file: %s\n", filename);

    if(fclose(file) != 0)
    {
        fprintf(stderr, "Error closing the file '%s': %s\n", filename, strerror(errno));
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

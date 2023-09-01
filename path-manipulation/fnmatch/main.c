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
#include <fnmatch.h>


int main(int argc, char *argv[])
{
    if(argc < 3)
    {
        fprintf(stderr, "Usage: %s pattern filename1 filename2 ...\n", argv[0]);
        return EXIT_FAILURE;
    }

    const char *pattern = argv[1];

    for(int i = 2; i < argc; i++)
    {
        const char *filename = argv[i];

        if(fnmatch(pattern, filename, 0) != 0)
        {
            printf("Filename '%s' doesn't match the pattern.\n", filename);
        }
        else
        {
            printf("Filename '%s' matches the pattern.\n", filename);
        }
    }

    return EXIT_SUCCESS;
}


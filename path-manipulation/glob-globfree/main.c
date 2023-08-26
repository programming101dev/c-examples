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
#include <glob.h>


int main(void)
{
    const char *pattern = "*.c";
    glob_t glob_result;

    if(glob(pattern, 0, NULL, &glob_result) != 0)
    {
        fprintf(stderr, "Error matching files.\n");
        return EXIT_FAILURE;
    }

    for(size_t i = 0; i < glob_result.gl_pathc; ++i)
    {
        printf("Matched file: %s\n", glob_result.gl_pathv[i]);
    }

    globfree(&glob_result);

    return EXIT_SUCCESS;
}


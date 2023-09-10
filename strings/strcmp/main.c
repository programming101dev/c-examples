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


// TODO take the 2 strings to compare in on the command line


int main(void)
{
    const char *str1 = "Hello, World!";
    const char *str2 = "Hello, World!";
    const char *str3 = "Hello, World?";
    int result;

    result = strcmp(str1, str2);

    if(result == 0)
    {
        printf("str1 and str2 are identical.\n");
    }
    else
    {
        printf("str1 and str2 are different (%d).\n", result);
    }

    result = strcmp(str1, str3);

    if(result == 0)
    {
        printf("str1 and str3 are identical.\n");
    }
    else
    {
        printf("str1 and str3 are different (%d).\n", result);
    }

    return EXIT_SUCCESS;
}

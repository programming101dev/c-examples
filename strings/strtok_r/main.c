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
    char str[] = "This,is,a,test,string";
    const char *delimiter = ",";
    char *token;
    char *saveptr; // Used to save the context between calls to strtok_r

    // First call to strtok_r with the input string and delimiter
    token = strtok_r(str, delimiter, &saveptr);

    while(token != NULL)
    {
        printf("Token: %s\n", token);

        // Subsequent calls to strtok_r with NULL as the first argument
        // to continue tokenizing the same string
        token = strtok_r(NULL, delimiter, &saveptr);
    }

    // NOTE that strtok_r is destructive to the original string
    printf("\nOriginal string \"%s\"\n", str);

    return EXIT_SUCCESS;
}

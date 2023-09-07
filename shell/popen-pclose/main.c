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


// TODO: pass the command on the command line


int main(void)
{
    char buffer[128];
    FILE *fp = popen("ls -l 2>&1", "r");

    if(fp == NULL)
    {
        perror("Error opening pipe");
        return 1;
    }

    printf("Output of \"ls -l\":\n");

    while(fgets(buffer, sizeof(buffer), fp) != NULL)
    {
        printf("%s", buffer);
    }

    if(pclose(fp) == -1)
    {
        perror("Error closing pipe");
        return 1;
    }

    return EXIT_SUCCESS;
}

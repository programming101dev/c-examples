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

int main(int argc, char *argv[])
{
    const char *default_command = "ls -l ~/*.txt";

    const char *command;
    char *saveptr;
    char *token;
    char *token_copy;
    char delimiter[] = " ";

    if(argc < 2)
    {
        command = default_command;
    }
    else
    {
        command = argv[1];
    }

    token_copy = strdup(command);
    if (token_copy == NULL) {
        printf("Memory allocation failed\n");
        return EXIT_FAILURE;
    }

    token = strtok_r(token_copy, delimiter, &saveptr);

    printf("Tokenized words:\n");

    while(token != NULL)
    {
        printf("Token: %s\n", token);
        token = strtok_r(NULL, delimiter, &saveptr);
    }

    free(token_copy);

    return EXIT_SUCCESS;
}

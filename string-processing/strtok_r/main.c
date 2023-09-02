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
#include <getopt.h>


static void usage(const char *program_name, int exit_code);


int main(int argc, char *argv[])
{
    const char *default_command = "ls -l ~/*.txt";

    const char *command;
    char *saveptr;
    char *token;
    char *token_copy;
    char delimiter[] = " ";
    int opt;

    while ((opt = getopt(argc, argv, "hc:")) != -1)
    {
        switch (opt)
        {
            case 'h':
                usage(argv[0], EXIT_SUCCESS);
            case 'c':
                command = optarg;
                break;
            default:
                usage(argv[0], EXIT_FAILURE);
        }
    }

    if (optind >= argc)
    {
        command = default_command;
    }

    token_copy = strdup(command);

    if(token_copy == NULL)
    {
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


static void usage(const char *program_name, int exit_code)
{
    fprintf(stderr, "Usage: %s [-c command]\n", program_name);
    fprintf(stderr, "Options:\n");
    fprintf(stderr, "  -c <command> : Specify the command (default: 'ls -l ~/*.txt')\n");
    fprintf(stderr, "  -h : Show help message\n");
    exit(exit_code);
}

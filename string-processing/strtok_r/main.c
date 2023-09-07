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


static void parse_arguments(int argc, char *argv[], char **command);
_Noreturn static void usage(const char *program_name, int exit_code, const char *message);


int main(int argc, char *argv[])
{
    char *command = NULL;
    char *saveptr;
    char *token;
    char *token_copy;
    char delimiter[] = " ";

    parse_arguments(argc, argv, &command);

    if(command == NULL)
    {
        command = strdup("ls -l ~/*.txt");
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


static void parse_arguments(int argc, char *argv[], char **command)
{
    int opt;

    opterr = 0;

    while((opt = getopt(argc, argv, "h")) != -1)
    {
        switch(opt)
        {
            case 'h':
            {
                usage(argv[0], EXIT_SUCCESS, NULL);
            }
            case '?':
            {
                char message[24];

                snprintf(message, sizeof(message), "Unknown option '-%c'.", optopt);
                usage(argv[0], EXIT_FAILURE, message);
            }
            default:
            {
                usage(argv[0], EXIT_FAILURE, NULL);
            }
        }
    }

    if(argc - optind > 1)
    {
        usage(argv[0], EXIT_FAILURE, "Too many unnamed arguments.");
    }

    if(argc - optind == 1)
    {
        *command = optarg;
    }
}


_Noreturn static void usage(const char *program_name, int exit_code, const char *message)
{
    if(message)
    {
        fprintf(stderr, "%s\n", message);
    }

    fprintf(stderr, "Usage: %s [-h] [command]\n", program_name);
    fputs("Options:\n", stderr);
    fputs("  -h            Display this help message\n", stderr);
    exit(exit_code);
}

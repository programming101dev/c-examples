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


#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


static void parse_arguments(int argc, char *argv[], char **command);
static void handle_arguments(const char *binary_name, const char *command);
_Noreturn static void usage(const char *program_name, int exit_code, const char *message);


#define UNKNOWN_OPTION_MESSAGE_LEN 24
#define BUFFER_LEN 128


int main(int argc, char *argv[])
{
    const char *const redirect = " 2>&1";
    char *command;
    char *redirected_command;
    char buffer[BUFFER_LEN];
    FILE *fp;

    command = NULL;
    parse_arguments(argc, argv, &command);
    handle_arguments(argv[0], command);
    redirected_command = (char *)malloc(strlen(command) + strlen(redirect) + 1);

    if(redirected_command == NULL)
    {
        perror("malloc");
        return EXIT_FAILURE;
    }

    strcpy(redirected_command, command);
    strcat(redirected_command, redirect);
    fp = popen(redirected_command, "r");
    free(redirected_command);

    if(fp == NULL)
    {
        perror("Error opening pipe");
        return EXIT_FAILURE;
    }

    printf("Output of \"%s\":\n", command);

    while(fgets(buffer, sizeof(buffer), fp) != NULL)
    {
        printf("%s", buffer);
    }

    if(pclose(fp) == -1)
    {
        perror("Error closing pipe");
        return EXIT_FAILURE;
    }

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
                char message[UNKNOWN_OPTION_MESSAGE_LEN];
                snprintf(message, sizeof(message), "Unknown option '-%c'.", optopt);
                usage(argv[0], EXIT_FAILURE, message);
            }
            default:
            {
                usage(argv[0], EXIT_FAILURE, NULL);
            }
        }
    }

    if(optind >= argc)
    {
        usage(argv[0], EXIT_FAILURE, "The group id is required");
    }

    if(optind < argc - 1)
    {
        usage(argv[0], EXIT_FAILURE, "Too many arguments.");
    }

    *command = argv[optind];
}


static void handle_arguments(const char *binary_name, const char *command)
{
    if(command == NULL)
    {
        usage(binary_name, EXIT_FAILURE, "The command is required.");
    }
}


_Noreturn static void usage(const char *program_name, int exit_code, const char *message)
{
    if(message)
    {
        fprintf(stderr, "%s\n", message);
    }

    fprintf(stderr, "Usage: %s [-h] <command>\n", program_name);
    fputs("Options:\n", stderr);
    fputs("  -h  Display this help message\n", stderr);
    exit(exit_code);
}

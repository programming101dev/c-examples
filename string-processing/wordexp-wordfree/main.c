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
#include <wordexp.h>
#include <getopt.h>


static void usage(const char *program_name);


int main(int argc, char *argv[])
{
    const char *default_command = "ls -l ~/*.txt";

    const char *command;
    wordexp_t result;
    int ret;

    int opt;
    while ((opt = getopt(argc, argv, "hc:")) != -1)
    {
        switch (opt)
        {
            case 'h':
                usage(argv[0]);
                return EXIT_SUCCESS;
            case 'c':
                command = optarg;
                break;
            default:
                usage(argv[0]);
                return EXIT_FAILURE;
        }
    }

    if (optind >= argc) {
        command = default_command;
    }

    ret = wordexp(command, &result, 0);

    if (ret != 0)
    {
        printf("Error expanding command: %d\n", ret);
        return EXIT_FAILURE;
    }

    printf("Expanded words:\n");

    for (size_t i = 0; i < result.we_wordc; ++i)
    {
        printf("Word %zu: %s\n", i, result.we_wordv[i]);
    }

    wordfree(&result);

    return EXIT_SUCCESS;
}


static void usage(const char *program_name)
{
    fprintf(stderr, "Usage: %s [-c command]\n", program_name);
    fprintf(stderr, "Options:\n");
    fprintf(stderr, "  -c <command> : Specify the command (default: 'ls -l ~/*.txt')\n");
    fprintf(stderr, "  -h : Show help message\n");
    exit(EXIT_FAILURE);
}

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
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


static void usage(const char *program_name, int exit_code, const char *message);


int main(int argc, char *argv[])
{
    int opt;
    bool option_alpha_set = false;
    bool option_beta_set = false;
    bool option_charlie_set = false;
    char *option_charlie_value = NULL;
    struct option long_options[] =
    {
            {"alpha", no_argument, NULL, 'a'},
            {"beta", no_argument, NULL, 'b'},
            {"charlie", required_argument, NULL, 'c'},
            {"help", no_argument, NULL, 'h'},
            {NULL, 0, NULL, 0}
    };

    opterr = 0;

    while ((opt = getopt_long(argc, argv, "abc:h", long_options, NULL)) != -1)
    {
        switch (opt)
        {
            case 'a':
                printf("Option 'alpha' is set\n");
                option_alpha_set = true;
                break;
            case 'b':
                printf("Option 'beta' is set\n");
                option_beta_set = true;
                break;
            case 'c':
                printf("Option 'charlie' is set with value '%s'\n", optarg);
                option_charlie_set = true;
                option_charlie_value = optarg;
                break;
            case 'h':
                usage(argv[0], EXIT_SUCCESS, NULL);
                break;
            default:
                usage(argv[0], EXIT_FAILURE, NULL);
        }
    }

    // Check if option 'charlie' is set
    if(!(option_charlie_set))
    {
        usage(argv[0], EXIT_FAILURE, "");
        return EXIT_FAILURE;
    }

    printf("Is option 'alpha' set?: %d\n", option_alpha_set);
    printf("Is option 'beta' set?: %d\n", option_beta_set);
    printf("Value of option 'charlie': %s\n", option_charlie_value);

    // Print remaining non-option arguments
    for (int i = optind; i < argc; i++)
    {
        printf("Non-option argument: '%s'\n", argv[i]);
    }

    return EXIT_SUCCESS;
}


static void usage(const char *program_name, int exit_code, const char *message)
{
    if(message)
    {
        fputs(message, stderr);
    }

    fprintf(stderr, "Usage: %s [OPTIONS] [arg1 arg2 ...]\n", program_name);
    fputs("Options:\n", stderr);
    fputs("  -a, --alpha            Option 'alpha'\n", stderr);
    fputs("  -b, --beta             Option 'beta'\n", stderr);
    fputs("  -c, --charlie value    Option 'charlie' (required) with value\n", stderr);
    fputs("  -h, --help             Display this help message\n", stderr);
    exit(exit_code);
}

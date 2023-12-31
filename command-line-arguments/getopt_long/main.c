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

static void           parse_arguments(int argc, char *argv[], bool *option_a_set, bool *option_b_set, char **option_c_value);
static void           handle_arguments(const char *binary_name, const char *option_c_value);
_Noreturn static void usage(const char *program_name, int exit_code, const char *message);

#define UNKNOWN_OPTION_MESSAGE_LEN 24

int main(int argc, char *argv[])
{
    bool  option_a_set;
    bool  option_b_set;
    char *option_c_value;

    option_a_set   = false;
    option_b_set   = false;
    option_c_value = NULL;
    parse_arguments(argc, argv, &option_a_set, &option_b_set, &option_c_value);
    handle_arguments(argv[0], option_c_value);
    printf("Is option 'a' set?: %d\n", option_a_set);
    printf("Is option 'b' set?: %d\n", option_b_set);
    printf("Value of option 'c': %s\n", option_c_value);

    for(int i = optind; i < argc; i++)
    {
        printf("Unnamed argument %d: '%s'\n", (i - optind + 1), argv[i]);
    }

    return EXIT_SUCCESS;
}

static void parse_arguments(int argc, char *argv[], bool *option_a_set, bool *option_b_set, char **option_c_value)
{
    static struct option long_options[] = {
        {"alpha",   no_argument,       NULL, 'a'},
        {"beta",    no_argument,       NULL, 'b'},
        {"charlie", required_argument, NULL, 'c'},
        {"help",    no_argument,       NULL, 'h'},
        {NULL,      0,                 NULL, 0  }
    };
    int opt;

    opterr = 0;

    while((opt = getopt_long(argc, argv, "habc:", long_options, NULL)) != -1)
    {
        switch(opt)
        {
            case 'a':
            {
                *option_a_set = true;
                break;
            }
            case 'b':
            {
                *option_b_set = true;
                break;
            }
            case 'c':
            {
                *option_c_value = optarg;
                break;
            }
            case 'h':
            {
                usage(argv[0], EXIT_SUCCESS, NULL);
            }
            case '?':
            {
                if(optopt == 'c')
                {
                    usage(argv[0], EXIT_FAILURE, "Option '-c' requires a value.");
                }
                else
                {
                    char message[UNKNOWN_OPTION_MESSAGE_LEN];

                    snprintf(message, sizeof(message), "Unknown option '-%c'.", optopt);
                    usage(argv[0], EXIT_FAILURE, message);
                }
            }
            default:
            {
                usage(argv[0], EXIT_FAILURE, NULL);
            }
        }
    }
}

static void handle_arguments(const char *binary_name, const char *option_c_value)
{
    if(option_c_value == NULL)
    {
        usage(binary_name, EXIT_FAILURE, "-c is required");
    }
}

_Noreturn static void usage(const char *program_name, int exit_code, const char *message)
{
    if(message)
    {
        fprintf(stderr, "%s\n", message);
    }

    fprintf(stderr, "Usage: %s [-h] [-a] [-b] -c <value> [arg1 arg2 ...]\n", program_name);
    fputs("Options:\n", stderr);
    fputs("  -h, --help                     Display this help message\n", stderr);
    fputs("  -a, --alpha                    Option 'a'\n", stderr);
    fputs("  -b, --beta                     Option 'b'\n", stderr);
    fputs("  -c <value>, --charlie <value>  Option 'c' (required) with value\n", stderr);
    exit(exit_code);
}

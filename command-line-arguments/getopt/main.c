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


#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


static void print_help(void);


int main(int argc, char *argv[])
{
    int opt;
    bool option_a_set;
    bool option_b_set;
    bool option_c_set;
    char *option_c_value;

    option_a_set = false;
    option_b_set = false;
    option_c_set = false;
    option_c_value = NULL;

    opterr = 0;

    while((opt = getopt(argc, argv, "abc:h")) != -1)
    {
        switch(opt)
        {
            case 'a':
                printf("Option 'a' is set\n");
                option_a_set = true;
                break;
            case 'b':
                printf("Option 'b' is set\n");
                option_b_set = true;
                break;
            case 'c':
                printf("Option 'c' is set with value '%s'\n", optarg);
                option_c_set = true;
                option_c_value = optarg;
                break;
            case 'h':
                print_help();
                return EXIT_SUCCESS;
            case '?':
                if(optopt == 'c')
                {
                    fprintf(stderr, "Option 'c' requires an argument.\n");
                }
                else
                {
                    fprintf(stderr, "Unknown option: '%c'\n", optopt);
                }

                print_help();
                return 1;
            default:
                break;
        }
    }

    // Check if option 'c' is set
    if(!(option_c_set))
    {
        fprintf(stderr, "Error: Option 'c' is required.\n");
        print_help();
        return EXIT_FAILURE;
    }

    printf("Is option 'a' set?: %d\n", option_a_set);
    printf("Is option 'b' set?: %d\n", option_b_set);
    printf("Value of option 'c': %s\n", option_c_value);

    // Print remaining non-option arguments
    for(int i = optind; i < argc; i++)
    {
        printf("Non-option argument: '%s'\n", argv[i]);
    }

    return EXIT_SUCCESS;
}


static void print_help(void)
{
    printf("Usage: example [-a] [-b] -c value [arg1 arg2 ...]\n");
    printf("Options:\n");
    printf("  -a             Option 'a'\n");
    printf("  -b             Option 'b'\n");
    printf("  -c value       Option 'c' (required) with value\n");
    printf("  -h             Display this help message\n");
}

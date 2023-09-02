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
#include <regex.h>
#include <getopt.h>


static void usage(const char *program_name, int exit_code);


int main(int argc, char *argv[])
{
    regex_t regex;
    int ret;
    char error_buffer[100];
    const char *pattern = "invalid[";

    int opt;
    while ((opt = getopt(argc, argv, "hp:")) != -1)
    {
        switch (opt)
        {
            case 'h':
                usage(argv[0], EXIT_SUCCESS);
            case 'p':
                pattern = optarg;
                break;
            default:
                usage(argv[0], EXIT_FAILURE);
        }
    }

    ret = regcomp(&regex, pattern, 0);

    if (ret != 0)
    {
        regerror(ret, &regex, error_buffer, sizeof(error_buffer));
        printf("Error compiling regex: %s\n", error_buffer);

        return EXIT_FAILURE;
    }

    printf("Regular expression compiled successfully\n");

    regfree(&regex);

    return EXIT_SUCCESS;
}

static void usage(const char *program_name, int exit_code)
{
    fprintf(stderr, "Usage: %s [-p pattern]\n", program_name);
    fprintf(stderr, "Options:\n");
    fprintf(stderr, "  -p <pattern> : Specify the regular expression pattern (default: 'invalid[')\n");
    fprintf(stderr, "  -h : Show help message\n");
    exit(exit_code);
}

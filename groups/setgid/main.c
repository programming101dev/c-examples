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
#include <unistd.h>
#include <getopt.h>


static void usage(const char *program_name, int exit_code, const char *message);


int main(int argc, char *argv[])
{
    char *endptr;
    gid_t new_gid = (gid_t)-1;

    int opt;
    while ((opt = getopt(argc, argv, "hg:")) != -1)
    {
        switch (opt)
        {
            case 'g':
                new_gid = (gid_t) strtol(optarg, &endptr, 10);

                if (*endptr != '\0')
                {
                    fprintf(stderr, "Invalid GID format: %s\n", optarg);
                    return EXIT_FAILURE;
                }
                break;
            case 'h':
                usage(argv[0], EXIT_SUCCESS, NULL);
                break;
            default:
                usage(argv[0], EXIT_FAILURE, NULL);
        }
    }

    if (optind < argc)
    {
        usage(argv[0], EXIT_FAILURE, "Unexpected extra arguments\n");
    }

    if (new_gid == (gid_t)-1)
    {
        usage(argv[0], EXIT_FAILURE, "");
    }

    if (setgid(new_gid) == -1)
    {
        perror("setgid");
        return EXIT_FAILURE;
    }

    printf("Real GID: %d\n", getgid());
    printf("Effective GID: %d\n", getegid());

    return EXIT_SUCCESS;
}

static void usage(const char *program_name, int exit_code, const char *message)
{
    if(message)
    {
        fputs(message, stderr);
    }

    fprintf(stderr, "Usage: %s -g <new_gid>\n", program_name);
    fputs("Options:\n", stderr);
    fputs("  -g <new_gid> : Specify the new GID\n", stderr);
    fputs("  -h : Show help message\n", stderr);
    exit(exit_code);
}

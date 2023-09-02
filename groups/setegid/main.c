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
#include <stdbool.h>

static void usage(const char *program_name, int exit_code);

int main(int argc, char *argv[])
{
    char *endptr;
    gid_t new_gid = (gid_t)-1;
    bool new_gid_set = false;

    int opt;
    while((opt = getopt(argc, argv, "hu:")) != -1)
    {
        switch(opt)
        {
            case 'h':
                usage(argv[0], EXIT_SUCCESS);
                break;
            case 'u':
                new_gid = (gid_t) strtol(optarg, &endptr, 10);
                new_gid_set = true;
                break;
            default:
                usage(argv[0], EXIT_FAILURE);
        }
    }

    if (!new_gid_set)
    {
        usage(argv[0], EXIT_FAILURE);
    }

    if(setegid(new_gid) == -1)
    {
        perror("setegid");
        return EXIT_FAILURE;
    }

    printf("Real GID: %d\n", getgid());
    printf("Effective GID: %d\n", getegid());

    return EXIT_SUCCESS;
}

static void usage(const char *program_name, int exit_code)
{
    fprintf(stderr, "Usage: %s -u <new_gid>\n", program_name);
    exit(exit_code);
}

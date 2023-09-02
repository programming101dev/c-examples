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
    gid_t new_gid;
    gid_t new_egid;

    int opt;
    while ((opt = getopt(argc, argv, "h")) != -1)
    {
        switch (opt)
        {
            case 'h':
                usage(argv[0], EXIT_SUCCESS, NULL);
                break;
            default:
                usage(argv[0], EXIT_FAILURE, NULL);
        }
    }

    if(argc != optind + 2)
    {
        usage(argv[0], EXIT_FAILURE, "Unexpected extra arguments\n");
    }

    new_gid = (gid_t)strtol(argv[optind], &endptr, 10);

    if(*endptr != '\0')
    {
        usage(argv[0], EXIT_FAILURE, "Invalid GID format");
    }

    new_egid = (gid_t)strtol(argv[optind + 1], &endptr, 10);

    if (*endptr != '\0')
    {
        fprintf(stderr, "Invalid Effective GID format: %s\n", argv[optind + 1]);
        return EXIT_FAILURE;
    }

    if (setregid(new_gid, new_egid) == -1)
    {
        perror("setregid");
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

    fprintf(stderr, "Usage: %s <new_gid> <new_egid>\n", program_name);
    exit(exit_code);
}

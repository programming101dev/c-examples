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


static void parse_arguments(int argc, char *argv[], gid_t *new_gid, gid_t *new_egid);
_Noreturn static void usage(const char *program_name, int exit_code, const char *message);


int main(int argc, char *argv[])
{
    gid_t new_gid;
    gid_t new_egid;

    parse_arguments(argc, argv, &new_gid, &new_egid);

    if(new_gid == (gid_t)-1)
    {
        usage(argv[0], EXIT_FAILURE, "");
    }

    if(new_egid == (gid_t)-1)
    {
        usage(argv[0], EXIT_FAILURE, "");
    }

    if(setregid(new_gid, new_egid) == -1)
    {
        perror("setregid");
        return EXIT_FAILURE;
    }

    printf("Real GID: %d\n", getgid());
    printf("Effective GID: %d\n", getegid());

    return EXIT_SUCCESS;
}


static void parse_arguments(int argc, char *argv[], gid_t *new_gid, gid_t *new_egid)
{
    int opt;
    char *endptr;

    opterr = 0;

    while((opt = getopt(argc, argv, "hg:e:")) != -1)
    {
        switch(opt)
        {
            case 'g':
            {
                *new_gid = (gid_t) strtol(argv[optind], &endptr, 10);

                if(*endptr != '\0')
                {
                    usage(argv[0], EXIT_FAILURE, "Invalid group_id format");
                }

                break;
            }
            case 'e':
            {
                *new_egid = (gid_t) strtol(argv[optind + 1], &endptr, 10);

                if(*endptr != '\0')
                {
                    usage(argv[0], EXIT_FAILURE, "Invalid effective group_id format");
                }

                break;
            }
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
}


_Noreturn  static void usage(const char *program_name, int exit_code, const char *message)
{
    if(message)
    {
        fprintf(stderr, "%s\n", message);
    }

    fprintf(stderr, "Usage: %s [-h] -g <group id> -e <effective group id>\n", program_name);
    fputs("Options:\n", stderr);
    fputs("  -h                       Display this help message\n", stderr);
    fputs("  -g <group id>            Specify the new group id\n", stderr);
    fputs("  -e <effective group id>  Specify the new effective group id\n", stderr);
    exit(exit_code);
}

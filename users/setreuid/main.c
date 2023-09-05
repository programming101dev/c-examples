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


static void parse_arguments(int argc, char *argv[], uid_t *new_uid, uid_t *new_euid);
_Noreturn static void usage(const char *program_name, int exit_code, const char *message);


int main(int argc, char *argv[])
{
    uid_t new_uid = (uid_t)-1;
    uid_t new_euid = (uid_t)-1;

    parse_arguments(argc, argv, &new_uid, &new_euid);

    if(setreuid(new_uid, new_euid) == -1)
    {
        perror("setreuid");
        return EXIT_FAILURE;
    }

    printf("Real UID: %d\n", getuid());
    printf("Effective UID: %d\n", geteuid());

    return EXIT_SUCCESS;
}


static void parse_arguments(int argc, char *argv[], uid_t *new_uid, uid_t *new_euid)
{
    int opt;
    char *endptr;

    opterr = 0;

    while((opt = getopt(argc, argv, "hu:e:")) != -1)
    {
        switch(opt)
        {
            case 'u':
            {
                *new_uid = (uid_t) strtol(optarg, &endptr, 10);

                if(*endptr != '\0')
                {
                    usage(argv[0], EXIT_FAILURE, "Invalid user id format");
                }
                break;
            }
            case 'e':
            {
                *new_euid = (uid_t) strtol(optarg, &endptr, 10);

                if(*endptr != '\0')
                {
                    usage(argv[0], EXIT_FAILURE, "Invalid effective user id format");
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

    if(optind < argc)
    {
        usage(argv[0], EXIT_FAILURE, "Unexpected extra arguments\n");
    }
}


_Noreturn  static void usage(const char *program_name, int exit_code, const char *message)
{
    if(message)
    {
        fprintf(stderr, "%s\n", message);
    }

    fprintf(stderr, "Usage: %s [-h] -u <user id> -e <effective user id>\n", program_name);
    fputs("Options:\n", stderr);
    fputs("  -h                      Display this help message\n", stderr);
    fputs("  -u <user id>            Specify the new user id\n", stderr);
    fputs("  -e <effective user id>  Specify the new effective user id\n", stderr);
    exit(exit_code);
}

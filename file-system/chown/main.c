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
#include <getopt.h>
#include <errno.h>
#include <unistd.h>


static void parse_arguments(int argc, char *argv[], char **file_path, uid_t *user_id, gid_t *group_id);
_Noreturn static void usage(const char *program_name, int exit_code, const char *message);


int main(int argc, char *argv[])
{
    char *file_path = NULL;
    uid_t user_id = (uid_t) - 1;
    gid_t group_id = (gid_t) - 1;

    parse_arguments(argc, argv, &file_path, &user_id, &group_id);

    if(file_path == NULL)
    {
        usage(argv[0], EXIT_FAILURE, "file path is required");
    }

    if(user_id == (uid_t) - 1)
    {
        usage(argv[0], EXIT_FAILURE, "-u is required");
    }

    if(group_id == (gid_t) - 1)
    {
        usage(argv[0], EXIT_FAILURE, "-g is required");
    }

    if(chown(file_path, user_id, group_id) == -1)
    {
        perror("chown");
        exit(EXIT_FAILURE);
    }

    printf("File ownership changed successfully.\n");

    return 0;
}


static void parse_arguments(int argc, char *argv[], char **file_path, uid_t *user_id, gid_t *group_id)
{
    int opt;

    opterr = 0;

    while((opt = getopt(argc, argv, "hu:g:")) != -1)
    {
        switch(opt)
        {
            case 'u':
            {
                errno = 0;
                *user_id = strtol(optarg, NULL, 10);

                if(errno != 0)
                {
                    perror("Invalid user ID");
                    exit(EXIT_FAILURE);
                }
                break;
            }
            case 'g':
            {
                errno = 0;
                *group_id = strtol(optarg, NULL, 10);

                if(errno != 0)
                {
                    perror("Invalid group ID");
                    exit(EXIT_FAILURE);
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

    if(optind >= argc)
    {
        usage(argv[0], EXIT_FAILURE, "The file path is required");
    }
    else if(optind < argc - 1)
    {
        usage(argv[0], EXIT_FAILURE, "Too many arguments.");
    }

    *file_path = argv[optind];
}


_Noreturn  static void usage(const char *program_name, int exit_code, const char *message)
{
    if(message)
    {
        fprintf(stderr, "%s\n", message);
    }

    fprintf(stderr, "Usage: %s [-h] -u <user id> -g <group id> <file path>\n", program_name);
    fputs("Options:\n", stderr);
    fputs("  -h              Display this help message\n", stderr);
    fputs("  -u <user id>    User ID\n", stderr);
    fputs("  -g <group id>   Group ID\n", stderr);
    exit(exit_code);
}

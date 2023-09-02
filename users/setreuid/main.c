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
    uid_t new_uid;
    uid_t new_euid;

    int opt;
    while ((opt = getopt(argc, argv, "hu:e:")) != -1)
    {
        switch (opt)
        {
            case 'h':
                usage(argv[0], EXIT_SUCCESS, NULL);
                return EXIT_SUCCESS;
            case 'u':
                new_uid = (uid_t)strtol(optarg, &endptr, 10);

                if (*endptr != '\0')
                {
                    fprintf(stderr, "Invalid UID format: %s\n", optarg);
                    return EXIT_FAILURE;
                }
                break;
            case 'e':
                new_euid = (uid_t)strtol(optarg, &endptr, 10);

                if (*endptr != '\0')
                {
                    fprintf(stderr, "Invalid Effective UID format: %s\n", optarg);
                    return EXIT_FAILURE;
                }
                break;
            default:
                usage(argv[0], EXIT_FAILURE, NULL);
        }
    }

    if (optind < argc)
    {
        usage(argv[0], EXIT_FAILURE, "Unexpected extra arguments\n");
    }

    if (setreuid(new_uid, new_euid) == -1)
    {
        perror("setreuid");
        return EXIT_FAILURE;
    }

    printf("Real UID: %d\n", getuid());
    printf("Effective UID: %d\n", geteuid());

    return EXIT_SUCCESS;
}


static void usage(const char *program_name, int exit_code, const char *message)
{
    if(message)
    {
        fputs(message, stderr);
    }

    fprintf(stderr, "Usage: %s -u <new_uid> -e <new_euid>\n", program_name);
    fputs("Options:\n", stderr);
    fputs("  -u <new_uid> : Specify the new UID\n", stderr);
    fputs("  -e <new_euid> : Specify the new effective UID\n", stderr);
    fputs("  -h : Show help message\n", stderr);
    exit(exit_code);
}

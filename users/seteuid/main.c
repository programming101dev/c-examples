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


static void usage(const char *program_name, int exit_code);


int main(int argc, char *argv[])
{
    char *endptr;
    uid_t new_uid;
    int opt;

    while ((opt = getopt(argc, argv, "hu:")) != -1)
    {
        switch (opt)
        {
            case 'h':
                usage(argv[0], EXIT_SUCCESS);
            case 'u':
                new_uid = (uid_t)strtol(optarg, &endptr, 10);

                if (*endptr != '\0')
                {
                    fprintf(stderr, "Invalid UID format: %s\n", optarg);
                    return EXIT_FAILURE;
                }

                if (seteuid(new_uid) == -1)
                {
                    perror("seteuid");
                    return EXIT_FAILURE;
                }

                printf("Real UID: %d\n", getuid());
                printf("Effective UID: %d\n", geteuid());

                return EXIT_SUCCESS;
            default:
                usage(argv[0], EXIT_FAILURE);
        }
    }

    usage(argv[0], EXIT_FAILURE);
    return EXIT_FAILURE;
}


static void usage(const char *program_name, int exit_code)
{
    fprintf(stderr, "Usage: %s -u <new_uid>\n", program_name);
    fprintf(stderr, "Options:\n");
    fprintf(stderr, "  -u <new_uid> : Specify the new UID\n");
    fprintf(stderr, "  -h : Show help message\n");
    exit(exit_code);
}

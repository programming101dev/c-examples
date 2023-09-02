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
    uid_t new_uid = (uid_t)-1;
    int opt;
    int has_new_uid_option = 0;

    while ((opt = getopt(argc, argv, "hu:")) != -1)
    {
        switch (opt)
        {
            case 'u':
                new_uid = (uid_t)strtol(optarg, NULL, 10);
                has_new_uid_option = 1;
                break;
            case 'h':
                usage(argv[0], EXIT_SUCCESS, NULL);
            default:
                usage(argv[0], EXIT_FAILURE, NULL);
        }
    }

    if(!(has_new_uid_option))
    {
        usage(argv[0], EXIT_FAILURE, "");
    }

    if (seteuid(new_uid) == -1)
    {
        perror("seteuid");
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

    fprintf(stderr, "Usage: %s -u <new_uid>\n", program_name);
    fputs("Options:\n", stderr);
    fputs("  -u <new_uid> : Specify the new UID\n", stderr);
    fputs("  -h : Show help message\n", stderr);
    exit(exit_code);
}

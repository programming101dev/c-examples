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
#include <pwd.h>
#include <errno.h>
#include <getopt.h>

static void usage(const char *program_name, int exit_code, const char *message);
static void print_entry(const struct passwd *entry);

int main(int argc, char *argv[])
{
    char *endptr;
    long int uid_long = -1;
    uid_t uid;
    int opt;

    while ((opt = getopt(argc, argv, "hu:")) != -1)
    {
        switch (opt)
        {
            case 'h':
                usage(argv[0], EXIT_SUCCESS, NULL);
            case 'u':
                uid_long = strtol(optarg, &endptr, 10);
                if (errno != 0 || *endptr != '\0')
                {
                    fprintf(stderr, "Invalid UID: %s\n", optarg);
                    usage(argv[0], EXIT_FAILURE, NULL);
                }
                break;
            default:
                usage(argv[0], EXIT_FAILURE, NULL);
        }
    }

    if (uid_long == -1)
    {
        usage(argv[0], EXIT_FAILURE, NULL);
    }

    uid = (uid_t)uid_long;
    struct passwd *user_info = getpwuid(uid);

    if (user_info != NULL)
    {
        print_entry(user_info);
    }
    else
    {
        printf("User with UID %d not found.\n", uid);
    }

    return EXIT_SUCCESS;
}

static void usage(const char *program_name, int exit_code, const char *message)
{
    if(message)
    {
        fputs(message, stderr);
    }

    fprintf(stderr, "Usage: %s -u <uid>\n", program_name);
    fputs("Options:\n", stderr);
    fputs("  -u <uid> : Specify the user's UID\n", stderr);
    fputs("  -h : Show help message\n", stderr);
    exit(exit_code);
}

static void print_entry(const struct passwd *entry)
{
    printf("Username: %s\n", entry->pw_name);
    printf("User ID (UID): %d\n", entry->pw_uid);
    printf("Group ID (GID): %d\n", entry->pw_gid);
    printf("Home directory: %s\n", entry->pw_dir);
    printf("Login shell: %s\n", entry->pw_shell);
    printf("-------------------------\n");
}


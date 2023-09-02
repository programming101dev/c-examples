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
#include <pwd.h>


static void usage(const char *program_name, int exit_code);
static void print_entry(const struct passwd *entry);


int main(int argc, char *argv[])
{
    const char *username;
    struct passwd *user_info;

    int opt;
    while ((opt = getopt(argc, argv, "hu:")) != -1)
    {
        switch (opt)
        {
            case 'u':
            {
                username = optarg;
                break;
            }
            case 'h':
            {
                usage(argv[0], EXIT_SUCCESS);
            }
            default:
            {
                usage(argv[0], EXIT_FAILURE);
            }
        }
    }

    if(username == NULL)
    {
        printf("Error: You must provide a username using -u option.\n");
        usage(argv[0], EXIT_FAILURE);
    }

    user_info = getpwnam(username);

    if(user_info != NULL)
    {
        print_entry(user_info);
    }
    else
    {
        printf("User '%s' not found.\n", username);
    }

    return EXIT_SUCCESS;
}


static void usage(const char *program_name, int exit_code)
{
    printf("Usage: %s -u <username>\n", program_name);
    printf("Options:\n");
    printf("  -u <username> : Specify the username\n");
    printf("  -h : Show help message\n");
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

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


#include <getopt.h>
#include <pwd.h>
#include <stdio.h>
#include <stdlib.h>


static void parse_arguments(int argc, char *argv[], char **user_name);
static void handle_arguments(const char *binary_name, const char *user_name);
_Noreturn static void usage(const char *program_name, int exit_code, const char *message);
static void print_entry(const struct passwd *entry);


#define UNKNOWN_OPTION_MESSAGE_LEN 24


int main(int argc, char *argv[])
{
    char          *user_name;
    struct passwd *user_info;

    user_name = NULL;
    parse_arguments(argc, argv, &user_name);
    handle_arguments(argv[0], user_name);
    user_info = getpwnam(user_name);

    if(user_info != NULL)
    {
        print_entry(user_info);
    }
    else
    {
        printf("User '%s' not found.\n", user_name);
    }

    return EXIT_SUCCESS;
}


static void parse_arguments(int argc, char *argv[], char **user_name)
{
    int opt;

    opterr = 0;

    while((opt = getopt(argc, argv, "h")) != -1)
    {
        switch(opt)
        {
            case 'h':
            {
                usage(argv[0], EXIT_SUCCESS, NULL);
            }
            case '?':
            {
                char message[UNKNOWN_OPTION_MESSAGE_LEN];
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
        usage(argv[0], EXIT_FAILURE, "The user name is required");
    }

    if(optind < argc - 1)
    {
        usage(argv[0], EXIT_FAILURE, "Too many arguments.");
    }

    *user_name = argv[optind];
}


static void handle_arguments(const char *binary_name, const char *user_name)
{
    if(user_name == NULL)
    {
        usage(binary_name, EXIT_FAILURE, "The user name are required.");
    }
}


_Noreturn static void usage(const char *program_name, int exit_code, const char *message)
{
    if(message)
    {
        fprintf(stderr, "%s\n", message);
    }

    fprintf(stderr, "Usage: %s [-h] <user name>\n", program_name);
    fputs("Options:\n", stderr);
    fputs("  -h  Display this help message\n", stderr);
    exit(exit_code);
}


static void print_entry(const struct passwd *entry)
{
    printf("Username: %s\n", entry->pw_name);
    printf("User ID (UID): %u\n", entry->pw_uid);
    printf("Group ID (GID): %u\n", entry->pw_gid);
    printf("Home directory: %s\n", entry->pw_dir);
    printf("Login shell: %s\n", entry->pw_shell);
    printf("-------------------------\n");
}

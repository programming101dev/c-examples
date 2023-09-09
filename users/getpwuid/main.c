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


#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <pwd.h>
#include <getopt.h>


static void parse_arguments(int argc, char *argv[], char **user_id);
static void handle_arguments(const char *binary_name, const char *user_id, uid_t *uid);
static uid_t parse_uid(const char *binary_name, const char *uid_str);
_Noreturn static void usage(const char *program_name, int exit_code, const char *message);
static void print_entry(const struct passwd *entry);


int main(int argc, char *argv[])
{
    char *user_id;
    uid_t uid;

    user_id = NULL;
    parse_arguments(argc, argv, &user_id);
    handle_arguments(argv[0], user_id, &uid);
    struct passwd *user_info = getpwuid(uid);

    if(user_info != NULL)
    {
        print_entry(user_info);
    }
    else
    {
        printf("User with UID %d not found.\n", uid);
    }

    return EXIT_SUCCESS;
}


static void parse_arguments(int argc, char *argv[], char **user_id)
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
        usage(argv[0], EXIT_FAILURE, "The user id is required");
    }
    else if(optind < argc - 1)
    {
        usage(argv[0], EXIT_FAILURE, "Too many arguments.");
    }

    *user_id = argv[optind];
}


static void handle_arguments(const char *binary_name, const char *user_id, uid_t *uid)
{
    if(user_id == NULL)
    {
        usage(binary_name, EXIT_FAILURE, "");
    }

    *uid = parse_uid(binary_name, user_id);
}


static uid_t parse_uid(const char *binary_name, const char *uid_str)
{
    char *endptr;
    long int parsed_uid;

    errno = 0;
    parsed_uid = strtol(uid_str, &endptr, 10);

    if(errno != 0)
    {
        usage(binary_name, EXIT_FAILURE, "Error parsing UID.");
    }

    // Check if there are any non-numeric characters in uid_str
    if(*endptr != '\0')
    {
        usage(binary_name, EXIT_FAILURE, "Invalid characters in UID.");
    }

    // Check if the UID is within the valid range
    if (parsed_uid < 0 || parsed_uid > UINT_MAX)
    {
        usage(binary_name, EXIT_FAILURE, "UID out of range.");
    }

    return (uid_t)parsed_uid;
}


_Noreturn static void usage(const char *program_name, int exit_code, const char *message)
{
    if(message)
    {
        fprintf(stderr, "%s\n", message);
    }

    fprintf(stderr, "Usage: %s [-h] <user id>\n", program_name);
    fputs("Options:\n", stderr);
    fputs("  -h  Display this help message\n", stderr);
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


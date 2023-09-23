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
#include <getopt.h>
#include <inttypes.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


static void parse_arguments(int argc, char *argv[], char **user_id);
static void handle_arguments(const char *binary_name, const char *user_id, uid_t *uid);
static uid_t get_uid_t_max(void);
static uid_t parse_uid_t(const char *binary_name, const char *str);
_Noreturn static void usage(const char *program_name, int exit_code, const char *message);


int main(int argc, char *argv[])
{
    char  *user_id;
    uid_t uid;
    user_id = NULL;
    parse_arguments(argc, argv, &user_id);
    handle_arguments(argv[0], user_id, &uid);
    if(setuid(uid) == -1)
    {
        perror("setuid");
        return EXIT_FAILURE;
    }
    printf("Real UID: %u\n", getuid());
    printf("Effective UID: %u\n", geteuid());
    return EXIT_SUCCESS;
}


static void parse_arguments(int argc, char *argv[], char **user_id)
{
    int opt;
    opterr     = 0;
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
        usage(binary_name, EXIT_FAILURE, "The user id are required.");
    }
    *uid = parse_uid_t(binary_name, user_id);
}


static uid_t get_uid_t_max(void)
{
    uid_t value;
    if(sizeof(uid_t) == sizeof(unsigned char))
    {
        value = (uid_t)UCHAR_MAX;
    }
    else if(sizeof(uid_t) == sizeof(unsigned short))
    {
        value = (uid_t)USHRT_MAX;
    }
    else if(sizeof(uid_t) == sizeof(unsigned int))
    {
        value = (uid_t)UINT_MAX;
    }
    else if(sizeof(uid_t) == sizeof(unsigned long))
    {
        value = (uid_t)ULONG_MAX;
    }
    else if(sizeof(uid_t) == sizeof(unsigned long long))
    {
        value = (uid_t)ULLONG_MAX;
    }
    else
    {
        // Handle other sizes or display an error message
        fprintf(stderr, "Unsupported size of uid_t\n");
        exit(EXIT_FAILURE);
    }
    return value;
}


static uid_t parse_uid_t(const char *binary_name, const char *str)
{
    uintmax_t max = get_uid_t_max();
    char      *endptr;
    uintmax_t parsed_value;
    errno         = 0;
    parsed_value  = strtoumax(str, &endptr, 10);
    if(errno != 0)
    {
        usage(binary_name, EXIT_FAILURE, "Error parsing uid_t.");
    }

    // Check if there are any non-numeric characters in the input string
    if(*endptr != '\0')
    {
        usage(binary_name, EXIT_FAILURE, "Invalid characters in input.");
    }
    if(parsed_value > max)
    {
        usage(binary_name, EXIT_FAILURE, "uid_t value out of range.");
    }
    return (uid_t)parsed_value;
}


_Noreturn static void usage(const char *program_name, int exit_code, const char *message)
{
    if(message)
    {
        fprintf(stderr, "%s\n", message);
    }
    fprintf(stderr, "Usage: [-h] %s <user id>\n", program_name);
    fputs("Options:\n", stderr);
    fputs("  -h  Display this help message\n", stderr);
    exit(exit_code);
}

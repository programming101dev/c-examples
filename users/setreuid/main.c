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


static void parse_arguments(int argc, char *argv[], char **user_id, char **euser_id);
static void handle_arguments(const char *binary_name, const char *user_id, char *euser_id, uid_t *uid, uid_t *euid);
static uid_t get_uid_t_max(void);
static uid_t parse_uid_t(const char *binary_name, const char *str);
_Noreturn static void usage(const char *program_name, int exit_code, const char *message);


#define UNKNOWN_OPTION_MESSAGE_LEN 24
#define BASE_TEN 10


int main(int argc, char *argv[])
{
    char  *user_id;
    char  *euser_id;
    uid_t uid;
    uid_t euid;

    user_id  = NULL;
    euser_id = NULL;
    parse_arguments(argc, argv, &user_id, &euser_id);
    handle_arguments(argv[0], user_id, euser_id, &uid, &euid);

    if(setreuid(uid, euid) == -1)
    {
        perror("setreuid");
        return EXIT_FAILURE;
    }

    printf("Real UID: %u\n", getuid());
    printf("Effective UID: %u\n", geteuid());

    return EXIT_SUCCESS;
}


static void parse_arguments(int argc, char *argv[], char **user_id, char **euser_id)
{
    int opt;

    opterr = 0;

    while((opt = getopt(argc, argv, "hu:e:")) != -1)
    {
        switch(opt)
        {
            case 'u':
            {
                *user_id = optarg;
                break;
            }
            case 'e':
            {
                *euser_id = optarg;
                break;
            }
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

    if(optind < argc)
    {
        usage(argv[0], EXIT_FAILURE, "Unexpected extra arguments");
    }
}


static void handle_arguments(const char *binary_name, const char *user_id, char *euser_id, uid_t *uid, uid_t *euid)
{
    if(user_id == NULL)
    {
        usage(binary_name, EXIT_FAILURE, "The user id are required.");
    }

    if(euser_id == NULL)
    {
        usage(binary_name, EXIT_FAILURE, "The effective user id are required.");
    }

    *uid  = parse_uid_t(binary_name, user_id);
    *euid = parse_uid_t(binary_name, euser_id);
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
    uintmax_t max;
    char      *endptr;
    uintmax_t parsed_value;

    max           = get_uid_t_max();
    errno         = 0;
    parsed_value  = strtoumax(str, &endptr, BASE_TEN);

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

    fprintf(stderr, "Usage: %s [-h] -u <user id> -e <effective user id>\n", program_name);
    fputs("Options:\n", stderr);
    fputs("  -h                      Display this help message\n", stderr);
    fputs("  -u <user id>            Specify the new user id\n", stderr);
    fputs("  -e <effective user id>  Specify the new effective user id\n", stderr);
    exit(exit_code);
}

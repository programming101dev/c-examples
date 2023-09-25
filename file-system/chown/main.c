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


static void parse_arguments(int argc, char *argv[], char **path, char **user_id, char **group_id);
static void handle_arguments(const char *binary_name, const char *path, const char *user_id, const char *group_id, uid_t *uid, gid_t *gid);
static uid_t get_uid_t_max(void);
static uid_t parse_uid_t(const char *binary_name, const char *str);
static gid_t get_gid_t_max(void);
static gid_t parse_gid_t(const char *binary_name, const char *str);
_Noreturn static void usage(const char *program_name, int exit_code, const char *message);


#define UNKNOWN_OPTION_MESSAGE_LEN 24
#define BASE_TEN 10


int main(int argc, char *argv[])
{
    char  *path;
    char  *user_id;
    char  *group_id;
    uid_t uid;
    gid_t gid;

    path     = NULL;
    user_id  = NULL;
    group_id = NULL;
    parse_arguments(argc, argv, &path, &user_id, &group_id);
    handle_arguments(argv[0], path, user_id, group_id, &uid, &gid);

    if(chown(path, uid, gid) == -1)
    {
        perror("chown");
        exit(EXIT_FAILURE);
    }

    printf("File ownership changed successfully.\n");

    return EXIT_SUCCESS;
}


static void parse_arguments(int argc, char *argv[], char **path, char **user_id, char **group_id)
{
    int opt;

    opterr = 0;

    while((opt = getopt(argc, argv, "hu:g:")) != -1)
    {
        switch(opt)
        {
            case 'u':
            {
                *user_id = optarg;
                break;
            }
            case 'g':
            {
                *group_id = optarg;
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

    if(optind >= argc)
    {
        usage(argv[0], EXIT_FAILURE, "The file path is required");
    }

    if(optind < argc - 1)
    {
        usage(argv[0], EXIT_FAILURE, "Too many arguments.");
    }

    *path = argv[optind];
}


static void handle_arguments(const char *binary_name, const char *path, const char *user_id, const char *group_id, uid_t *uid, gid_t *gid)
{
    if(path == NULL)
    {
        usage(binary_name, EXIT_FAILURE, "The path is required.");
    }

    if(user_id == NULL)
    {
        usage(binary_name, EXIT_FAILURE, "The user id is required.");
    }

    if(group_id == NULL)
    {
        usage(binary_name, EXIT_FAILURE, "The group id is required.");
    }

    *uid = parse_uid_t(binary_name, user_id);
    *gid = parse_gid_t(binary_name, group_id);
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


static gid_t get_gid_t_max(void)
{
    gid_t value;

    if(sizeof(gid_t) == sizeof(unsigned char))
    {
        value = (gid_t)UCHAR_MAX;
    }
    else if(sizeof(gid_t) == sizeof(unsigned short))
    {
        value = (gid_t)USHRT_MAX;
    }
    else if(sizeof(gid_t) == sizeof(unsigned int))
    {
        value = UINT_MAX;
    }
    else if(sizeof(gid_t) == sizeof(unsigned long))
    {
        value = (gid_t)ULONG_MAX;
    }
    else if(sizeof(gid_t) == sizeof(unsigned long long))
    {
        value = (gid_t)ULLONG_MAX;
    }
    else
    {
        fprintf(stderr, "Unsupported size of gid_t\n");
        exit(EXIT_FAILURE);
    }

    return value;
}


static gid_t parse_gid_t(const char *binary_name, const char *str)
{
    gid_t     max;
    char      *endptr;
    uintmax_t parsed_value;

    max           = get_gid_t_max();
    errno         = 0;
    parsed_value  = strtoumax(str, &endptr, BASE_TEN);

    if(errno != 0)
    {
        usage(binary_name, EXIT_FAILURE, "Error parsing gid_t.");
    }

    // Check if there are any non-numeric characters in the input string
    if(*endptr != '\0')
    {
        usage(binary_name, EXIT_FAILURE, "Invalid characters in input.");
    }

    if(parsed_value > max)
    {
        usage(binary_name, EXIT_FAILURE, "gid_t value out of range.");
    }

    return (gid_t)parsed_value;
}


_Noreturn static void usage(const char *program_name, int exit_code, const char *message)
{
    if(message)
    {
        fprintf(stderr, "%s\n", message);
    }

    fprintf(stderr, "Usage: %s [-h] -u <user id> -g <group id> <path>\n", program_name);
    fputs("Options:\n", stderr);
    fputs("  -h              Display this help message\n", stderr);
    fputs("  -u <user id>    User ID\n", stderr);
    fputs("  -g <group id>   Group ID\n", stderr);
    exit(exit_code);
}

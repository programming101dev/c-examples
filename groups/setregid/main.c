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
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


static void parse_arguments(int argc, char *argv[], char **group_id, char **egroup_id);
static void handle_arguments(const char *binary_name, const char *group_id, const char *egroup_id, gid_t *gid, gid_t *egid);
static gid_t parse_gid(const char *binary_name, const char *gid_str);
_Noreturn static void usage(const char *program_name, int exit_code, const char *message);


int main(int argc, char *argv[])
{
    char *group_id;
    char *egroup_id;
    gid_t gid;
    gid_t egid;

    group_id = NULL;
    egroup_id = NULL;
    parse_arguments(argc, argv, &group_id, &egroup_id);
    handle_arguments(argv[0], group_id, egroup_id, &gid, &egid);

    if(setregid(gid, egid) == -1)
    {
        perror("setregid");
        return EXIT_FAILURE;
    }

    printf("Real GID: %d\n", getgid());
    printf("Effective GID: %d\n", getegid());

    return EXIT_SUCCESS;
}


static void parse_arguments(int argc, char *argv[], char **group_id, char **egroup_id)
{
    int opt;

    opterr = 0;

    while((opt = getopt(argc, argv, "hg:e:")) != -1)
    {
        switch(opt)
        {
            case 'g':
            {
                *group_id = optarg;
                break;
            }
            case 'e':
            {
                *egroup_id = optarg;
                break;
            }
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
}


static void handle_arguments(const char *binary_name, const char *group_id, const char *egroup_id, gid_t *gid, gid_t *egid)
{
    if(group_id == NULL)
    {
        usage(binary_name, EXIT_FAILURE, "The group id is required.");
    }

    if(egroup_id == NULL)
    {
        usage(binary_name, EXIT_FAILURE, "The effective group id is required.");
    }

    *gid = parse_gid(binary_name, group_id);
    *egid = parse_gid(binary_name, egroup_id);
}


static gid_t parse_gid(const char *binary_name, const char *gid_str)
{
    char *endptr;
    long int parsed_gid;

    errno = 0;
    parsed_gid = strtol(gid_str, &endptr, 10);

    if(errno != 0)
    {
        usage(binary_name, EXIT_FAILURE, "Error parsing GID.");
    }

    // Check if there are any non-numeric characters in uid_str
    if(*endptr != '\0')
    {
        usage(binary_name, EXIT_FAILURE, "Invalid characters in GID.");
    }

    // Check if the UID is within the valid range
    if (parsed_gid < 0 || parsed_gid > UINT_MAX)
    {
        usage(binary_name, EXIT_FAILURE, "GID out of range.");
    }

    return (gid_t)parsed_gid;
}


_Noreturn static void usage(const char *program_name, int exit_code, const char *message)
{
    if(message)
    {
        fprintf(stderr, "%s\n", message);
    }

    fprintf(stderr, "Usage: %s [-h] -g <group id> -e <effective group id>\n", program_name);
    fputs("Options:\n", stderr);
    fputs("  -h                       Display this help message\n", stderr);
    fputs("  -g <group id>            Specify the new group id\n", stderr);
    fputs("  -e <effective group id>  Specify the new effective group id\n", stderr);
    exit(exit_code);
}

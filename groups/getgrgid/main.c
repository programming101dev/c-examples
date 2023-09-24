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
#include <grp.h>
#include <inttypes.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>


static void parse_arguments(int argc, char *argv[], char **group_id);
static void handle_arguments(const char *binary_name, const char *group_id, gid_t *gid);
static gid_t get_gid_t_max(void);
static gid_t parse_gid_t(const char *binary_name, const char *gid_str);
_Noreturn static void usage(const char *program_name, int exit_code, const char *message);
static void print_entry(const struct group *entry);


#define UNKNOWN_OPTION_MESSAGE_LEN 24
#define BASE_TEN 10


int main(int argc, char *argv[])
{
    char         *group_id;
    gid_t        gid;
    struct group *group_info;

    group_id = NULL;
    parse_arguments(argc, argv, &group_id);
    handle_arguments(argv[0], group_id, &gid);
    group_info = getgrgid(gid);

    if(group_info == NULL)
    {
        printf("Group with GID %u not found.\n", gid);
    }
    else
    {
        print_entry(group_info);
    }

    return EXIT_SUCCESS;
}


static void parse_arguments(int argc, char *argv[], char **group_id)
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
        usage(argv[0], EXIT_FAILURE, "The group id is required");
    }
    else if(optind < argc - 1)
    {
        usage(argv[0], EXIT_FAILURE, "Too many arguments.");
    }

    *group_id = argv[optind];
}


static void handle_arguments(const char *binary_name, const char *group_id, gid_t *gid)
{
    if(group_id == NULL)
    {
        usage(binary_name, EXIT_FAILURE, "The group id is required.");
    }

    *gid = parse_gid_t(binary_name, group_id);
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
        return (gid_t)UINT_MAX;
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

    max          = get_gid_t_max();
    errno        = 0;
    parsed_value = strtoumax(str, &endptr, BASE_TEN);

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

    fprintf(stderr, "Usage: %s [-h] <group id>\n", program_name);
    fputs("Options:\n", stderr);
    fputs("  -h  Display this help message\n", stderr);
    exit(exit_code);
}


static void print_entry(const struct group *entry)
{
    printf("Group name: %s\n", entry->gr_name);
    printf("Group ID (GID): %u\n", entry->gr_gid);
    printf("Group Members:\n");

    if(entry->gr_mem != NULL)
    {
        for(int i = 0; entry->gr_mem[i] != NULL; i++)
        {
            printf(" - %s\n", entry->gr_mem[i]);
        }

        printf("-------------------------\n");
    }
    else
    {
        printf("No members found.\n");
    }
}

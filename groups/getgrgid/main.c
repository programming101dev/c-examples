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
#include <grp.h>
#include <errno.h>
#include <getopt.h>


static void parse_arguments(int argc, char *argv[], gid_t *gid);
_Noreturn static void usage(const char *program_name, int exit_code, const char *message);
static void print_entry(const struct group *entry);


int main(int argc, char *argv[])
{
    gid_t gid;
    struct group *group_info;

    parse_arguments(argc, argv, &gid);
    group_info = getgrgid(gid);

    if(group_info == NULL)
    {
        printf("Group with GID %d not found.\n", gid);
    }
    else
    {
        print_entry(group_info);
    }

    return EXIT_SUCCESS;
}


static void parse_arguments(int argc, char *argv[], gid_t *gid)
{
    char *endptr;
    long int gid_long;
    int opt;

    opterr = 0;

    while((opt = getopt(argc, argv, "h")) != -1)
    {
        switch(opt)
        {
            case 'h':
            {
                usage(argv[0], EXIT_SUCCESS, NULL);
                break;
            }
            case '?':
            {
                char message[24];

                snprintf(message, sizeof(message), "Unknown option '-%c'.", optopt);
                usage(argv[0], EXIT_FAILURE, message);
                break;
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

    errno = 0;
    gid_long = strtol(argv[optind], &endptr, 10);

    if(errno != 0 || *endptr != '\0')
    {
        usage(argv[0], EXIT_FAILURE, "Invalid gid");
    }

    *gid = (gid_t) gid_long;
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
    char **members;

    printf("Group name: %s\n", entry->gr_name);
    printf("Group ID (GID): %d\n", entry->gr_gid);
    printf("Group Members:\n");
    members = entry->gr_mem;

    if(members != NULL)
    {
        // TODO: this crashes on macOS
        while(*members != NULL)
        {
            // TODO: this crashes on macOS
            printf(" - %s\n", *members);
            members++;
        }

        printf("-------------------------\n");
    }
}

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


static void usage(const char *program_name);
static void print_entry(const struct group *entry);


int main(int argc, char *argv[])
{
    int opt;
    char *endptr;
    long int gid_long;
    gid_t gid;
    struct group *group_info;

    while((opt = getopt(argc, argv, "h")) != -1)
    {
        switch(opt)
        {
            case 'h':
            {
                printf("Usage: %s <gid>\n", argv[0]);
                return EXIT_SUCCESS;
            }
            default:
            {
                fprintf(stderr, "Usage: %s <gid> [-h]\n", argv[0]);
                return EXIT_FAILURE;
            }
        }
    }

    if(optind != argc - 1)
    {
        usage(argv[0]);
    }

    errno = 0;
    gid_long = strtol(argv[optind], &endptr, 10);

    if(errno != 0 || *endptr != '\0')
    {
        printf("Invalid GID: %s\n", argv[optind]);
        return EXIT_FAILURE;
    }

    gid = (gid_t)gid_long;
    group_info = getgrgid(gid);

    if(group_info != NULL)
    {
        print_entry(group_info);
    }
    else
    {
        printf("Group with GID %d not found.\n", gid);
    }

    return EXIT_SUCCESS;
}


static void usage(const char *program_name)
{
    printf("Usage: %s <gid>\n", program_name);
    printf("Options:\n");
    printf("  -h : Display this help message\n");
    exit(EXIT_FAILURE);
}


static void print_entry(const struct group *entry)
{
    char **members;

    printf("Group name: %s\n", entry->gr_name);
    printf("Group ID (GID): %d\n", entry->gr_gid);
    printf("Group Members:\n");
    members = entry->gr_mem;

    while(*members != NULL)
    {
        printf(" - %s\n", *members);
        members++;
    }

    printf("-------------------------\n");
}

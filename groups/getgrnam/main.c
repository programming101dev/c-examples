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
#include <getopt.h>


static void usage(const char *program_name);
static void print_entry(const struct group *entry);


int main(int argc, char *argv[])
{
    const char *groupname;
    struct group *group_info;

    int opt;
    while ((opt = getopt(argc, argv, "hg:")) != -1)
    {
        switch (opt)
        {
            case 'h':
                usage(argv[0]);
                return EXIT_SUCCESS;
            case 'g':
                groupname = optarg;
                break;
            default:
                usage(argv[0]);
                return EXIT_FAILURE;
        }
    }

    if (optind < argc)
    {
        fprintf(stderr, "Unexpected extra arguments\n");
        usage(argv[0]);
        return EXIT_FAILURE;
    }

    group_info = getgrnam(groupname);

    if (group_info != NULL)
    {
        print_entry(group_info);
    }
    else
    {
        printf("Group '%s' not found.\n", groupname);
    }

    return EXIT_SUCCESS;
}


static void usage(const char *program_name)
{
    fprintf(stderr, "Usage: %s -g <groupname>\n", program_name);
    fprintf(stderr, "Options:\n");
    fprintf(stderr, "  -g <groupname> : Specify the groupname\n");
    fprintf(stderr, "  -h : Show help message\n");
    exit(EXIT_FAILURE);
}


static void print_entry(const struct group *entry)
{
    char **members;

    printf("Group name: %s\n", entry->gr_name);
    printf("Group ID (GID): %d\n", entry->gr_gid);
    printf("Group Members:\n");
    members = entry->gr_mem;

    while (*members != NULL)
    {
        printf(" - %s\n", *members);
        members++;
    }

    printf("-------------------------\n");
}

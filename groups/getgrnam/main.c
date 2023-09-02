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


static void usage(const char *program_name, int exit_code, const char *message);
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
            case 'g':
                groupname = optarg;
                break;
            case 'h':
                usage(argv[0], EXIT_SUCCESS, NULL);
                break;
            default:
                usage(argv[0], EXIT_FAILURE, NULL);
        }
    }

    if(optind < argc)
    {
        usage(argv[0], EXIT_FAILURE, "Unexpected extra arguments\n");
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


static void usage(const char *program_name, int exit_code, const char *message)
{
    if(message)
    {
        fputs(message, stderr);
    }

    fprintf(stderr, "Usage: %s -g <groupname>\n", program_name);
    fputs("Options:\n", stderr);
    fputs("  -g <groupname> : Specify the groupname\n", stderr);
    fputs("  -h : Show help message\n", stderr);
    exit(exit_code);
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

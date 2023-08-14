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


static void print_entry(const struct group *entry);


int main(int argc, char *argv[])
{
    const char *groupname;
    struct group *group_info;

    if(argc != 2)
    {
        printf("Usage: %s <groupname>\n", argv[0]);
        return EXIT_FAILURE;
    }

    groupname = argv[1];
    group_info = getgrnam(groupname);

    if(group_info != NULL)
    {
        print_entry(group_info);
    }
    else
    {
        printf("Group '%s' not found.\n", groupname);
    }

    return EXIT_SUCCESS;
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

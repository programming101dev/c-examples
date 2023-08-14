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


static void print_entries(void);
static void print_entry(const struct group *entry);


int main(void)
{
    struct group *entry;

    // Use getgrent to retrieve all group information
    printf("All group accounts:\n");
    print_entries();

    // Rewind the group database
    setgrent();
    entry = getgrent();

    if(entry != NULL)
    {
        print_entry(entry);
    }

    // Close the group database after usage
    endgrent();

    return EXIT_SUCCESS;
}

static void print_entries(void)
{
    struct group *entry;

    while((entry = getgrent()) != NULL)
    {
        print_entry(entry);
    }
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

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

#include <grp.h>
#include <stdio.h>
#include <stdlib.h>

static void print_entries(void);
static void print_entry(const struct group *entry);

int main(void)
{
    const struct group *entry;

    // Use getgrent to retrieve all group information
    printf("All group accounts:\n");
    print_entries();

    // Rewind the group database
    setgrent();
    entry = getgrent();    // cppcheck-suppress getgrentCalled

    if(entry != NULL)
    {
        print_entry(entry);
    }

    endgrent();

    return EXIT_SUCCESS;
}

static void print_entries(void)
{
    const struct group *entry;

    while((entry = getgrent()) != NULL)    // cppcheck-suppress getgrentCalled
    {
        print_entry(entry);
    }
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

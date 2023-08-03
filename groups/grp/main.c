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

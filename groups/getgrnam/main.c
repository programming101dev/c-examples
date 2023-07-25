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

    groupname  = argv[1];
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

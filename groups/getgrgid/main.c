#include <stdio.h>
#include <stdlib.h>
#include <grp.h>
#include <errno.h>


static void print_entry(const struct group *entry);


int main(int argc, char *argv[])
{
    char *endptr;
    long int gid_long;
    gid_t gid;
    struct group *group_info;

    if(argc != 2)
    {
        printf("Usage: %s <gid>\n", argv[0]);
        return EXIT_FAILURE;
    }

    errno = 0;
    gid_long = strtol(argv[1], &endptr, 10);

    if(errno != 0 || *endptr != '\0')
    {
        printf("Invalid GID: %s\n", argv[1]);
        return EXIT_FAILURE;
    }

    gid = (gid_t) gid_long;
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

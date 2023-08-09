#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


int main(int argc, char *argv[])
{
    char *endptr;
    gid_t new_gid;

    if(argc != 2)
    {
        fprintf(stderr, "Usage: %s <new_gid>\n", argv[0]);
        return EXIT_FAILURE;
    }

    new_gid = (gid_t) strtol(argv[1], &endptr, 10);

    if(*endptr != '\0')
    {
        fprintf(stderr, "Invalid GID format: %s\n", argv[1]);
        return EXIT_FAILURE;
    }

    if(setegid(new_gid) == -1)
    {
        perror("setegid");
        return EXIT_FAILURE;
    }

    printf("Real GID: %d\n", getgid());
    printf("Effective GID: %d\n", getegid());

    return EXIT_SUCCESS;
}

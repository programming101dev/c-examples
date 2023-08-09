#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


int main(int argc, char *argv[])
{
    char *endptr;
    uid_t new_uid;
    uid_t new_euid;

    if(argc != 3)
    {
        fprintf(stderr, "Usage: %s <new_gid> <new_egid>\n", argv[0]);
        return EXIT_FAILURE;
    }

    new_uid = (uid_t) strtol(argv[1], &endptr, 10);

    if(*endptr != '\0')
    {
        fprintf(stderr, "Invalid UID format: %s\n", argv[1]);
        return EXIT_FAILURE;
    }

    new_euid = (uid_t) strtol(argv[2], &endptr, 10);

    if(*endptr != '\0')
    {
        fprintf(stderr, "Invalid Effective UID format: %s\n", argv[2]);
        return EXIT_FAILURE;
    }

    if(setreuid(new_uid, new_euid) == -1)
    {
        perror("setreuid");
        return EXIT_FAILURE;
    }

    printf("Real UID: %d\n", getuid());
    printf("Effective UID: %d\n", geteuid());

    return EXIT_SUCCESS;
}

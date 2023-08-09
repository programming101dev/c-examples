#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


int main(int argc, char *argv[])
{
    char *endptr;
    uid_t new_uid;

    if(argc != 2)
    {
        fprintf(stderr, "Usage: %s <new_uid>\n", argv[0]);
        return EXIT_FAILURE;
    }

    new_uid = (uid_t) strtol(argv[1], &endptr, 10);

    if(*endptr != '\0')
    {
        fprintf(stderr, "Invalid UID format: %s\n", argv[1]);
        return EXIT_FAILURE;
    }

    if(seteuid(new_uid) == -1)
    {
        perror("seteuid");
        return EXIT_FAILURE;
    }

    printf("Real UID: %d\n", getuid());
    printf("Effective UID: %d\n", geteuid());

    return EXIT_SUCCESS;
}

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


int main(void)
{
    long max_hostname_length;
    char *hostname;
    int result;

    max_hostname_length = sysconf(_SC_HOST_NAME_MAX);

    if(max_hostname_length <= 0)
    {
        perror("sysconf");
        return EXIT_FAILURE;
    }

    hostname = malloc(max_hostname_length + 1);

    if(hostname == NULL)
    {
        perror("malloc");
        return EXIT_FAILURE;
    }

    result = gethostname(hostname, max_hostname_length);

    if (result == 0)
    {
        printf("Hostname: %s\n", hostname);
    }
    else
    {
        perror("gethostname");
        free(hostname);
        return EXIT_FAILURE;
    }

    free(hostname);

    return EXIT_SUCCESS;
}

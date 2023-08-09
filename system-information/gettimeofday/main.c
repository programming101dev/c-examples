#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>


int main(void)
{
    struct timeval tv;

    if(gettimeofday(&tv, NULL) == -1)
    {
        perror("gettimeofday");
        return EXIT_FAILURE;
    }

    printf("Seconds: %lld\nMicroseconds: %lld\n", (long long) tv.tv_sec, (long long) tv.tv_usec);

    return EXIT_SUCCESS;
}

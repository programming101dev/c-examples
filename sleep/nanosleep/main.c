#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>


int main(void)
{
    struct timespec tim, rem;

    tim.tv_sec = 3; // seconds
    tim.tv_nsec = 0; // nanoseconds

    printf("Starting program...\n");
    printf("Sleeping for 3 seconds...\n");

    while(nanosleep(&tim, &rem) == -1)
    {
        if(errno == EINTR)
        {
            tim = rem; // If interrupted by a signal, assign the remaining time to tim
            continue;
        }
        else
        {
            perror("Unexpected error from nanosleep");
            return -1;
        }
    }

    printf("Program woke up!\n");

    return EXIT_SUCCESS;
}

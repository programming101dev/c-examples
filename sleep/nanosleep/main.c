#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>


int main(void)
{
    struct timespec tim;
    struct timespec rem;

    tim.tv_sec  = 1; // seconds
    tim.tv_nsec = 500000000; // 0.5 seconds in nanoseconds
    printf("Starting program...\n");
    printf("Sleeping for 1.5 seconds...\n");

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
            return EXIT_FAILURE;
        }
    }

    printf("Program woke up!\n");

    return EXIT_SUCCESS;
}

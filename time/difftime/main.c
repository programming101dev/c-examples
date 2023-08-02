#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>


int main(void)
{
    time_t start_time;
    time_t end_time;
    double diff_time;

    // Get the start time
    if (time(&start_time) == -1)
    {
        perror("Error getting start time");
        return EXIT_FAILURE;
    }

    // Simulate some time passing
    sleep(5); // Sleep for 5 seconds

    // Get the end time
    if (time(&end_time) == -1)
    {
        perror("Error getting end time");
        return EXIT_FAILURE;
    }

    // Calculate the time difference
    if (end_time == (time_t)-1 || start_time == (time_t)-1)
    {
        perror("Invalid time values");
        return EXIT_FAILURE;
    }

    diff_time = difftime(end_time, start_time);
    printf("Time difference: %.2f seconds\n", diff_time);

    return EXIT_SUCCESS;
}

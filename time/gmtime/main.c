#include <stdio.h>
#include <stdlib.h>
#include <time.h>


int main(void)
{
    time_t rawtime;
    struct tm *timeinfo;
    char buffer[80];

    // Get the current time
    if(time(&rawtime) == -1)
    {
        perror("Error getting current time");
        return EXIT_FAILURE;
    }

    // Convert the time to a local time representation
    timeinfo = gmtime(&rawtime);

    if(timeinfo == NULL)
    {
        perror("Error converting to GMT/UTC time");
        return EXIT_FAILURE;
    }

    // Format the time
    if(strftime(buffer, sizeof(buffer), "GMT/UTC Time: %Y-%m-%d %H:%M:%S\n", timeinfo) == 0)
    {
        perror("Error formatting time");
        return EXIT_FAILURE;
    }

    printf("%s", buffer);

    return EXIT_SUCCESS;
}

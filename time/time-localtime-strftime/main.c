#include <stdio.h>
#include <stdlib.h>
#include <time.h>


int main(void)
{
    time_t rawtime;
    struct tm *timeinfo;
    char buffer[80];

    if(time(&rawtime) == -1)
    {
        perror("Error getting current time");
        return EXIT_FAILURE;
    }

    timeinfo = localtime(&rawtime);

    if(timeinfo == NULL)
    {
        perror("Error converting to local time");
        return EXIT_FAILURE;
    }

    if(strftime(buffer, sizeof(buffer), "Formatted date: %Y-%m-%d %H:%M:%S\n", timeinfo) == 0)
    {
        perror("Error formatting time");
        return EXIT_FAILURE;
    }

    printf("%s", buffer);

    return EXIT_SUCCESS;
}

#include <stdio.h>
#include <stdlib.h>
#include <time.h>


int main(void)
{
    struct tm timeinfo;
    time_t result_time;
    char buffer[80];

    timeinfo.tm_sec = 17;
    timeinfo.tm_min = 30;
    timeinfo.tm_hour = 12;
    timeinfo.tm_mday = 1;
    timeinfo.tm_mon = 6;
    timeinfo.tm_year = 122; // Year since 1900 (2022)
    timeinfo.tm_isdst = -1; // Daylight Saving Time setting (let the system determine)

    // Convert the timeinfo struct to a time_t value
    result_time = mktime(&timeinfo);

    if(result_time == -1)
    {
        perror("Error converting time");
        return EXIT_FAILURE;
    }

    strftime(buffer, sizeof(buffer), "Converted time: %Y-%m-%d %H:%M:%S\n", &timeinfo);
    printf("%s", buffer);

    return EXIT_SUCCESS;
}

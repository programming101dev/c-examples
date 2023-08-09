#include <stdio.h>
#include <stdlib.h>
#include <time.h>


int main(void)
{
    const char *time_str = "2023-08-02 14:30:00";
    const char *format = "%Y-%m-%d %H:%M:%S";
    struct tm timeinfo;
    time_t result_time;

    if(strptime(time_str, format, &timeinfo) == NULL)
    {
        fprintf(stderr, "Error parsing time\n");
        return EXIT_FAILURE;
    }

    // Manually set the daylight saving time flag since strptime doesn't set it
    timeinfo.tm_isdst = -1;

    // Convert the timeinfo struct to a time_t value
    result_time = mktime(&timeinfo);

    if(result_time == -1)
    {
        fprintf(stderr, "Error converting time\n");
        return EXIT_FAILURE;
    }

    printf("Parsed Time: %04d-%02d-%02d %02d:%02d:%02d\n",
           timeinfo.tm_year + 1900,
           timeinfo.tm_mon + 1,
           timeinfo.tm_mday,
           timeinfo.tm_hour,
           timeinfo.tm_min,
           timeinfo.tm_sec);
    printf("Time in seconds since epoch: %lld\n", (long long) result_time);

    return EXIT_SUCCESS;
}

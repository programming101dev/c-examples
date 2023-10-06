/*
 * This code is licensed under the Attribution-NonCommercial-NoDerivatives 4.0 International license.
 *
 * Author: D'Arcy Smith (ds@programming101.dev)
 *
 * You are free to:
 *   - Share: Copy and redistribute the material in any medium or format.
 *   - Under the following terms:
 *       - Attribution: You must give appropriate credit, provide a link to the license, and indicate if changes were made.
 *       - NonCommercial: You may not use the material for commercial purposes.
 *       - NoDerivatives: If you remix, transform, or build upon the material, you may not distribute the modified material.
 *
 * For more details, please refer to the full license text at:
 * https://creativecommons.org/licenses/by-nc-nd/4.0/
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define BASE_YEAR 1900

int main(int argc, char *argv[])
{
    // TODO pass this in on the command line
    const char *time_str = "2023-08-02 14:30:00";
    const char *format   = "%Y-%m-%d %H:%M:%S";
    struct tm   timeinfo;
    time_t      result_time;

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

    printf("Parsed Time: %04d-%02d-%02d %02d:%02d:%02d\n", timeinfo.tm_year + BASE_YEAR, timeinfo.tm_mon + 1, timeinfo.tm_mday, timeinfo.tm_hour, timeinfo.tm_min, timeinfo.tm_sec);
    printf("Time in seconds since epoch: %lld\n", (long long)result_time);

    return EXIT_SUCCESS;
}

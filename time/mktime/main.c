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


#define BUFFER_LEN 80


int main(void)
{
    struct tm timeinfo;
    time_t    result_time;
    char      buffer[BUFFER_LEN];

    // NOLINTBEGIN(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers)
    timeinfo.tm_sec   = 17;
    timeinfo.tm_min   = 30;
    timeinfo.tm_hour  = 12;
    timeinfo.tm_mday  = 1;
    timeinfo.tm_mon   = 6;
    timeinfo.tm_year  = 122; // Year since 1900 (2022)
    timeinfo.tm_isdst = -1; // Daylight Saving Time setting (let the system determine)
// NOLINTEND(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers)

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

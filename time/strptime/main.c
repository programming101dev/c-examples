/*
 * Copyright 2026 D'Arcy Smith
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define BASE_YEAR 1900

int main(void)
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

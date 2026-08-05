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
    timeinfo.tm_year  = 122;    // Year since 1900 (2022)
    timeinfo.tm_isdst = -1;     // Daylight Saving Time setting (let the system determine)
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

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
    time_t           rawtime;
    const struct tm *timeinfo;
    char             buffer[BUFFER_LEN];

    // Get the current time
    if(time(&rawtime) == -1)
    {
        perror("Error getting current time");
        return EXIT_FAILURE;
    }

    // Convert the time to a local time representation
    timeinfo = gmtime(&rawtime);    // cppcheck-suppress gmtimeCalled

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

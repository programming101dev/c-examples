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
    struct tm        result;
    char             buffer[BUFFER_LEN];

    if(time(&rawtime) == -1)
    {
        perror("Error getting current time");
        return EXIT_FAILURE;
    }

    timeinfo = localtime_r(&rawtime, &result);

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

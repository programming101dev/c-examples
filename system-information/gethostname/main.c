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
#include <unistd.h>

int main(void)
{
    size_t max_hostname_length;
    long   length;
    char  *hostname;
    int    result;

    length = sysconf(_SC_HOST_NAME_MAX);

    if(length <= 0)
    {
        perror("sysconf");
        return EXIT_FAILURE;
    }

    max_hostname_length = (size_t)length;
    hostname            = (char *)malloc(max_hostname_length + 1);

    if(hostname == NULL)
    {
        perror("malloc");
        return EXIT_FAILURE;
    }

    result = gethostname(hostname, max_hostname_length);

    if(result == 0)
    {
        printf("Hostname: %s\n", hostname);
    }
    else
    {
        perror("gethostname");
        free(hostname);
        return EXIT_FAILURE;
    }

    free(hostname);

    return EXIT_SUCCESS;
}

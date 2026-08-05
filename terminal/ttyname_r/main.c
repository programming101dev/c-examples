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
    long   length;
    size_t max_name_length;
    char  *ttyname_buf;

    length = sysconf(_SC_TTY_NAME_MAX);

    if(length == -1)
    {
        perror("Error getting maximum terminal name length");
        return 1;
    }

    max_name_length = (size_t)length;
    ttyname_buf     = (char *)calloc(max_name_length + 1, sizeof(char));

    if(ttyname_buf == NULL)
    {
        perror("Error allocating memory for terminal name buffer");
        return EXIT_FAILURE;
    }

    if(ttyname_r(STDIN_FILENO, ttyname_buf, max_name_length + 1) == 0)
    {
        printf("The terminal associated with stdin is: %s\n", ttyname_buf);
    }
    else
    {
        perror("stdin is not associated with a terminal");
    }

    free(ttyname_buf);    // Free the allocated memory

    return EXIT_SUCCESS;
}

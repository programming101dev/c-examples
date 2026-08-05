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

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    int result;

    result = fgetc(stdout);

    if(result == EOF)
    {
        if(errno != 0)
        {
            int current_errno = errno;
            errno             = 0;

            if(fprintf(stderr, "errno: %d\n", current_errno) < 0)
            {
                printf("could not fprintf: %d\n", errno);
            }
        }
    }

    return EXIT_SUCCESS;
}

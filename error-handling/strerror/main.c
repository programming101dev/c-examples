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
#include <string.h>

int main(void)
{
    int result;

    errno  = 0;
    result = fgetc(stdout);

    if(result == EOF)
    {
        if(errno != 0)
        {
            const char *msg;
            int         current_erro_no;

            current_erro_no = errno;
            errno           = 0;
            msg             = strerror(current_erro_no);

            if(errno != 0)
            {
                perror("strerror");
                return EXIT_FAILURE;
            }

            errno = 0;

            if(fprintf(stderr, "There was an error with fgetc: %s\n", msg) < 0)
            {
                perror("fprintf");
                return EXIT_FAILURE;
            }
        }
    }

    return EXIT_SUCCESS;
}

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
    size_t max_login_length;
    char  *login_name;
    int    result;

    length = sysconf(_SC_LOGIN_NAME_MAX);

    if(length <= 0)
    {
        perror("sysconf");
        return EXIT_FAILURE;
    }

    max_login_length = (size_t)length;
    login_name       = (char *)malloc(max_login_length + 1);

    if(login_name == NULL)
    {
        perror("malloc");
        return EXIT_FAILURE;
    }

    result = getlogin_r(login_name, max_login_length);

    if(result == 0)
    {
        printf("Logged-in user: %s\n", login_name);
    }
    else
    {
        perror("getlogin_r");
        free(login_name);
        return EXIT_FAILURE;
    }

    free(login_name);

    return EXIT_SUCCESS;
}

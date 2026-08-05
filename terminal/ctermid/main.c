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

int main(void)
{
    const char *terminal_name;

    terminal_name = ctermid(NULL);    // cppcheck-suppress ctermidCalled

    if(*terminal_name != '\0')
    {
        printf("The current terminal name is: %s\n", terminal_name);
    }
    else
    {
        printf("Failed to get the current terminal name.\n");
    }

    return EXIT_SUCCESS;
}

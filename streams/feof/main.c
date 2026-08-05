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

#define BUFFER_SIZE 256

int main(void)
{
    char buffer[BUFFER_SIZE];

    printf("Enter some text (press Ctrl+D to stop):\n");

    while(!feof(stdin))
    {
        if(fgets(buffer, BUFFER_SIZE, stdin) != NULL)
        {
            printf("You entered: %s", buffer);
        }
    }

    printf("\nReached the end of input.\n");

    return EXIT_SUCCESS;
}

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

#include "testlib.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void display(const char *msg)
{
    char *uppercase_msg;

    uppercase_msg = strdup(msg);

    if(uppercase_msg == NULL)
    {
        fprintf(stderr, "Error: Memory allocation failed.\n");
        return;
    }

    // Convert the message to uppercase
    for(int i = 0; uppercase_msg[i]; i++)
    {
        uppercase_msg[i] = (char)toupper(uppercase_msg[i]);
    }

    printf("testlib-2: display: %s\n", uppercase_msg);
    free(uppercase_msg);
}

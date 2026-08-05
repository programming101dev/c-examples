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

static int is_little_endian(void) __attribute__((const));

int main(void)
{
    if(is_little_endian())    // cppcheck-suppress knownConditionTrueFalse
    {
        printf("Machine is little-endian.\n");
    }
    else
    {
        printf("Machine is big-endian.\n");
    }

    return 0;
}

static int is_little_endian(void)
{
    unsigned int         value;
    const unsigned char *byte_ptr;

    // Create a 32-bit integer with a known value
    value = 1;

    // Obtain the first byte of the integer
    byte_ptr = (unsigned char *)&value;

    // If the first byte (lowest address) contains 1, the machine is little-endian
    return (*byte_ptr == 1);    // cppcheck-suppress knownConditionTrueFalse
}

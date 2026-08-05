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
#include <string.h>

int main(void)
{
    const int data1[] = {
        1,
        2,
        3,
        4,
    };
    const int data2[] = {
        1,
        2,
        3,
        4,
    };
    const int data3[] = {
        1,
        2,
        3,
        4,
    };
    int result1;
    int result2;

    // Compare data1 and data2
    result1 = memcmp(data1, data2, sizeof(data1));

    if(result1 == 0)
    {
        printf("data1 and data2 are identical.\n");
    }
    else
    {
        printf("data1 and data2 are different.\n");
    }

    result2 = memcmp(data1, data3, sizeof(data1));

    if(result2 == 0)
    {
        printf("data1 and data3 are identical.\n");
    }
    else
    {
        printf("data1 and data3 are different.\n");
    }

    return EXIT_SUCCESS;
}

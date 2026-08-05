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

#define NUM_ELEMENTS 4

int main(void)
{
    // Source data
    int source_data[] = {
        1,
        2,
        3,
        4,
    };
    size_t num_elements;
    int    destination_buffer[NUM_ELEMENTS];

    num_elements = sizeof(source_data) / sizeof(source_data[0]);
    memcpy(destination_buffer, source_data, num_elements * sizeof(source_data[0]));
    printf("Copied elements: ");

    for(size_t i = 0; i < num_elements; i++)
    {
        printf("%d ", destination_buffer[i]);
    }

    printf("\n");

    return EXIT_SUCCESS;
}

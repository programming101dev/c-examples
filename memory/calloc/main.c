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

static void print_array(const int *arr, size_t size);

int main(void)
{
    // TODO pass this in on the command line
    const size_t num_elements = 5;
    int         *dynamic_array;

    dynamic_array = (int *)calloc(num_elements, sizeof(dynamic_array[0]));

    if(dynamic_array == NULL)
    {
        perror("Memory allocation failed\n");
        return EXIT_FAILURE;
    }

    print_array(dynamic_array, num_elements);
    free(dynamic_array);

    return EXIT_SUCCESS;
}

static void print_array(const int *arr, size_t size)
{
    for(size_t i = 0; i < size; i++)
    {
        printf("%d ", arr[i]);
    }

    printf("\n");
}

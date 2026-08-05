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
#include <time.h>

static void print_array(const int *arr, size_t size);
static void fill_with_random_ints(int *arr, size_t size);

int main(void)
{
    // TODO pass this in on the command line
    const size_t num_elements = 5;
    int         *dynamic_array;

    dynamic_array = (int *)malloc(num_elements * sizeof(dynamic_array[0]));

    if(dynamic_array == NULL)
    {
        perror("Memory allocation failed\n");
        return EXIT_FAILURE;
    }

    printf("Array after malloc:\n");

#if defined(__GNUC__) && !defined(__clang__)
    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wmaybe-uninitialized"
#endif
    print_array(dynamic_array, num_elements);
#if defined(__GNUC__) && !defined(__clang__)
    #pragma GCC diagnostic pop
#endif

    fill_with_random_ints(dynamic_array, num_elements);
    printf("Array after filling with random integers:\n");
    print_array(dynamic_array, num_elements);
    memset(dynamic_array, 0, num_elements * sizeof(dynamic_array[0]));
    printf("Array after memset:\n");
    print_array(dynamic_array, num_elements);
    free(dynamic_array);

    return EXIT_SUCCESS;
}

static void print_array(const int *arr, size_t size)
{
    for(size_t i = 0; i < size; i++)
    {
#if defined(__GNUC__) && !defined(__clang__)
    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wanalyzer-use-of-uninitialized-value"
#endif
#ifndef __clang_analyzer__
        printf("%d ", arr[i]);    // NOLINT(clang-analyzer-core.CallAndMessage)
#endif
#if defined(__GNUC__) && !defined(__clang__)
    #pragma GCC diagnostic pop
#endif
    }

    printf("\n");
}

static void fill_with_random_ints(int *arr, size_t size)
{
    for(size_t i = 0; i < size; i++)
    {
        arr[i] = (int)arc4random();
    }
}

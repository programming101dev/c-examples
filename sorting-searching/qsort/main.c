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

static int intcmp(const void *a, const void *b);

int main(void)
{
    // NOLINTBEGIN(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers)
    int arr[] = {5, 2, 8, 1, 3};
    // NOLINTEND(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers)
    size_t size;

    size = sizeof(arr) / sizeof(arr[0]);
    printf("Original array: ");

    for(size_t i = 0; i < size; i++)
    {
        printf("%d ", arr[i]);
    }

    printf("\n");

    // Using qsort to sort the array
    qsort(arr, size, sizeof(int), intcmp);
    printf("Sorted array: ");

    for(size_t i = 0; i < size; i++)
    {
        printf("%d ", arr[i]);
    }

    printf("\n");

    return EXIT_SUCCESS;
}

static int intcmp(const void *a, const void *b)
{
    return (*(const int *)a - *(const int *)b);
}

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

#ifdef bsearch
    #undef bsearch
#endif

static int    intcmp(const void *a, const void *b);
static size_t find_index(const int *arr, size_t size, const int *element);

/* TODO: take the number to find and the numbers to search on the command line. */

int main(void)
{
    /* NOLINTBEGIN(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers) */
    int arr[] = {5, 2, 8, 1, 3};
    /* NOLINTEND(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers) */
    size_t     size;
    const int *result;
    int        target;

    size = sizeof(arr) / sizeof(arr[0]);
    printf("Unsorted array: ");

    for(size_t i = 0; i < size; i++)
    {
        printf("%d ", arr[i]);
    }

    printf("\n");

    qsort(arr, size, sizeof(int), intcmp);

    printf("Sorted array: ");

    for(size_t i = 0; i < size; i++)
    {
        printf("%d ", arr[i]);
    }

    printf("\n");

    target = 3;
    result = (const int *)bsearch(&target, arr, size, sizeof(int), intcmp);

    if(result != NULL)
    {
        size_t index;

        index = find_index(arr, size, result);
        printf("Element %d found at index %zu\n", target, index);
    }
    else
    {
        printf("Element %d not found\n", target);
    }

    return EXIT_SUCCESS;
}

static int intcmp(const void *a, const void *b)
{
    const int lhs = *(const int *)a;
    const int rhs = *(const int *)b;

    if(lhs < rhs)
    {
        return -1;
    }

    if(lhs > rhs)
    {
        return 1;
    }

    return 0;
}

static size_t find_index(const int *arr, size_t size, const int *element)
{
    for(size_t i = 0; i < size; i++)
    {
        if(&arr[i] == element)
        {
            return i;
        }
    }

    return size;
}

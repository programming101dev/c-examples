/*
 * This code is licensed under the Attribution-NonCommercial-NoDerivatives 4.0 International license.
 *
 * Author: D'Arcy Smith (ds@programming101.dev)
 *
 * You are free to:
 *   - Share: Copy and redistribute the material in any medium or format.
 *   - Under the following terms:
 *       - Attribution: You must give appropriate credit, provide a link to the license, and indicate if changes were made.
 *       - NonCommercial: You may not use the material for commercial purposes.
 *       - NoDerivatives: If you remix, transform, or build upon the material, you may not distribute the modified material.
 *
 * For more details, please refer to the full license text at:
 * https://creativecommons.org/licenses/by-nc-nd/4.0/
 */

#include <stdio.h>
#include <stdlib.h>

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
    result = bsearch(&target, arr, size, sizeof(int), intcmp);

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

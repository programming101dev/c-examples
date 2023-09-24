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


static int intcmp(const void *a, const void *b);


// TODO take the number to to find and the numbers to search on the command line


int main(void)
{
    // NOLINTBEGIN(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers)
    int    arr[] = {
            5,
            2,
            8,
            1,
            3
    };
    // NOLINTEND(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers)
    size_t size  = sizeof(arr) / sizeof(arr[0]);
    int    *result;
    int    target;
    printf("Unsorted array: ");
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
    target = 3;

    // Using bsearch to search for the target element
    result = (int *)bsearch(&target, arr, size, sizeof(int), intcmp);
    if(result != NULL)
    {
        printf("Element %d found at index %ld\n", target, result - arr);
    }
    else
    {
        printf("Element %d not found\n", target);
    }
    return EXIT_SUCCESS;
}


static int intcmp(const void *a, const void *b)
{
    return (*(const int *)a - *(const int *)b);
}

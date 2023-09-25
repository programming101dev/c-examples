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


static void print_array(const int *arr, size_t size);


#define NUM_ELEMENTS 5


int main(void)
{
    const size_t num_elements = NUM_ELEMENTS;
    int          *dynamic_array;

    dynamic_array = (int *)malloc(num_elements * sizeof(dynamic_array[0]));

    if(dynamic_array == NULL)
    {
        perror("Memory allocation failed\n");
        return EXIT_FAILURE;
    }

#if defined(__GNUC__) && !defined(__clang__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wmaybe-uninitialized"
#endif
    print_array(dynamic_array, num_elements);
#if defined(__GNUC__) && !defined(__clang__)
#pragma GCC diagnostic pop
#endif

    free(dynamic_array);

    return EXIT_SUCCESS;
}


static void print_array(const int *arr, size_t size)
{
    for(size_t i = 0; i < size; i++)
    {
        printf("%d ", arr[i]);      // NOLINT(clang-analyzer-core.CallAndMessage)
    }

    printf("\n");
}

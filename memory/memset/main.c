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
    srand((unsigned int)time(NULL));    // NOLINT(cert-msc32-c,cert-msc51-cpp)

    for(size_t i = 0; i < size; i++)
    {
        arr[i] = rand();
    }
}

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

static void initialize_array(int *array, size_t num_elements);
static void print_array(const char *message, const int *array, size_t num_elements);
static int *resize_array(int *array, size_t new_num_elements);

#define NUM_ELEMENTS 5

int main(void)
{
    int   *dynamic_array;
    size_t num_elements;

    num_elements  = NUM_ELEMENTS;
    dynamic_array = (int *)malloc(num_elements * sizeof(int));

    if(dynamic_array == NULL)
    {
        perror("Memory allocation failed\n");
        return EXIT_FAILURE;
    }

    initialize_array(dynamic_array, num_elements);
    print_array("Initial elements of the array: ", dynamic_array, num_elements);
    num_elements  = (size_t)NUM_ELEMENTS * 2;
    dynamic_array = resize_array(dynamic_array, num_elements);
    initialize_array(&dynamic_array[NUM_ELEMENTS], num_elements - NUM_ELEMENTS);
    print_array("Resized elements of the array: ", dynamic_array, num_elements);
    free(dynamic_array);

    return EXIT_SUCCESS;
}

static void initialize_array(int *array, size_t num_elements)
{
    for(size_t i = 0; i < num_elements; i++)
    {
        array[i] = (int)i + 1;
    }
}

static void print_array(const char *message, const int *array, size_t num_elements)
{
    printf("%s", message);

    for(size_t i = 0; i < num_elements; i++)
    {
        printf("%d ", array[i]);
    }

    printf("\n");
}

static int *resize_array(int *array, size_t new_num_elements)
{
    int *resized_array;

    resized_array = (int *)realloc(array, new_num_elements * sizeof(int));

    if(resized_array == NULL)
    {
        perror("Memory reallocation failed\n");
        free(array);
        exit(EXIT_FAILURE);
    }

    return resized_array;
}

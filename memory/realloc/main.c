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

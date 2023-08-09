#include <stdio.h>
#include <stdlib.h>


static void initialize_array(int *array, int num_elements);
static void print_array(const char *message, const int *array, int num_elements);
static int *resize_array(int *array, int old_num_elements, int new_num_elements);


int main(void)
{
    int *dynamic_array;
    int num_elements;

    num_elements = 5;
    dynamic_array = malloc(num_elements * sizeof(int));

    if(dynamic_array == NULL)
    {
        perror("Memory allocation failed\n");
        return EXIT_FAILURE;
    }

    initialize_array(dynamic_array, num_elements);
    print_array("Initial elements of the array: ", dynamic_array, num_elements);

    num_elements  = 10;
    dynamic_array = resize_array(dynamic_array, 5, num_elements);
    initialize_array(&dynamic_array[5], num_elements - 5);
    print_array("Resized elements of the array: ", dynamic_array, num_elements);

    free(dynamic_array);

    return EXIT_SUCCESS;
}


static void initialize_array(int *array, int num_elements)
{
    for(int i = 0; i < num_elements; i++)
    {
        array[i] = i + 1;
    }
}


static void print_array(const char *message, const int *array, int num_elements)
{
    printf("%s", message);

    for(int i = 0; i < num_elements; i++)
    {
        printf("%d ", array[i]);
    }

    printf("\n");
}


#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
static int *resize_array(int *array, int old_num_elements, int new_num_elements)
{
    int *resized_array;

    resized_array = realloc(array, new_num_elements * sizeof(int));

    if(resized_array == NULL)
    {
        perror("Memory reallocation failed\n");
        free(array);
        exit(EXIT_FAILURE);
    }

    return resized_array;
}
#pragma GCC diagnostic pop

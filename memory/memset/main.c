#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int main(void)
{
    const int num_elements = 5;
    int *dynamic_array;

    dynamic_array = calloc(num_elements, sizeof(dynamic_array[0]));

    if(dynamic_array == NULL)
    {
        perror("Memory allocation failed\n");
        return EXIT_FAILURE;
    }

    memset(dynamic_array, 0, num_elements * sizeof(dynamic_array[0]));
    free(dynamic_array);

    return EXIT_SUCCESS;
}
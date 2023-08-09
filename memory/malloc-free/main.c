#include <stdio.h>
#include <stdlib.h>


int main(void)
{
    const int num_elements = 5;
    int *dynamic_array;

    dynamic_array = malloc(num_elements * sizeof(dynamic_array[0]));

    if(dynamic_array == NULL)
    {
        perror("Memory allocation failed\n");
        return EXIT_FAILURE;
    }

    for(int i = 0; i < num_elements; i++)
    {
        dynamic_array[i] = 0;
    }

    free(dynamic_array);

    return EXIT_SUCCESS;
}

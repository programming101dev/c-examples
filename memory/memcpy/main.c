#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int main(void)
{
    // Source data
    int source_data[] = {1, 2, 3, 4, 5};
    int num_elements;
    int destination_buffer[5];

    num_elements = sizeof(source_data) / sizeof(source_data[0]);
    memcpy(destination_buffer, source_data, num_elements * sizeof(source_data[0]));
    printf("Copied elements: ");

    for(int i = 0; i < num_elements; i++)
    {
        printf("%d ", destination_buffer[i]);
    }

    printf("\n");

    return EXIT_SUCCESS;
}

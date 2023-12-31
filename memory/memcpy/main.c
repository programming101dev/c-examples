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

#define NUM_ELEMENTS 4

int main(void)
{
    // Source data
    int source_data[] = {
        1,
        2,
        3,
        4,
    };
    size_t num_elements;
    int    destination_buffer[NUM_ELEMENTS];

    num_elements = sizeof(source_data) / sizeof(source_data[0]);
    memcpy(destination_buffer, source_data, num_elements * sizeof(source_data[0]));
    printf("Copied elements: ");

    for(size_t i = 0; i < num_elements; i++)
    {
        printf("%d ", destination_buffer[i]);
    }

    printf("\n");

    return EXIT_SUCCESS;
}

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


int main(void)
{
    int data1[] = {
            1,
            2,
            3,
            4,
    };
    int data2[] = {
            1,
            2,
            3,
            4,
    };
    int data3[] = {
            1,
            2,
            3,
            4,
    };
    int result1;
    int result2;

    // Compare data1 and data2
    result1 = memcmp(data1, data2, sizeof(data1));
    if(result1 == 0)
    {
        printf("data1 and data2 are identical.\n");
    }
    else
    {
        printf("data1 and data2 are different.\n");
    }
    result2 = memcmp(data1, data3, sizeof(data1));
    if(result2 == 0)
    {
        printf("data1 and data3 are identical.\n");
    }
    else
    {
        printf("data1 and data3 are different.\n");
    }
    return EXIT_SUCCESS;
}

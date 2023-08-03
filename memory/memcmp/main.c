#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int main(void)
{
    int data1[] = {1, 2, 3, 4, 5};
    int data2[] = {1, 2, 3, 4, 5};
    int data3[] = {1, 2, 3, 5, 4};
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
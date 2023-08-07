#include <stdio.h>


int is_little_endian(void);


int is_little_endian(void)
{
    // Create a 32-bit integer with a known value
    unsigned int value = 1;

    // Obtain the first byte of the integer
    unsigned char *byte_ptr = (unsigned char *)&value;

    // If the first byte (lowest address) contains 1, the machine is little-endian
    return (*byte_ptr == 1);
}

int main(void)
{
    if (is_little_endian())
    {
        printf("Machine is little-endian.\n");
    }
    else
    {
        printf("Machine is big-endian.\n");
    }

    return 0;
}

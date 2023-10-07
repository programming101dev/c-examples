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

static int is_little_endian(void) __attribute__((const));

int main(void)
{
    if(is_little_endian())    // cppcheck-suppress knownConditionTrueFalse
    {
        printf("Machine is little-endian.\n");
    }
    else
    {
        printf("Machine is big-endian.\n");
    }

    return 0;
}

static int is_little_endian(void)
{
    unsigned int         value;
    const unsigned char *byte_ptr;

    // Create a 32-bit integer with a known value
    value = 1;

    // Obtain the first byte of the integer
    byte_ptr = (unsigned char *)&value;

    // If the first byte (lowest address) contains 1, the machine is little-endian
    return (*byte_ptr == 1);    // cppcheck-suppress knownConditionTrueFalse
}

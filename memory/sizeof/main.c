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

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>

int main(void)
{
    printf("Size of pointer: %zu\n", sizeof(void *));
    printf("Size of char: %zu\n", sizeof(char));
    printf("Size of wchar_t: %zu\n", sizeof(wchar_t));
    printf("Size of int8_t: %zu\n", sizeof(int8_t));
    printf("Size of uint8_t: %zu\n", sizeof(uint8_t));
    printf("Size of int16_t: %zu\n", sizeof(int16_t));
    printf("Size of uint16_t: %zu\n", sizeof(uint16_t));
    printf("Size of int32_t: %zu\n", sizeof(int32_t));
    printf("Size of uint32_t: %zu\n", sizeof(uint32_t));
    printf("Size of int: %zu\n", sizeof(int));
    printf("Size of unsigned int: %zu\n", sizeof(unsigned int));
    printf("Size of long: %zu\n", sizeof(long));
    printf("Size of unsigned long: %zu\n", sizeof(unsigned long));
    printf("Size of long long: %zu\n", sizeof(long long));
    printf("Size of unsigned long long: %zu\n", sizeof(unsigned long long));
    printf("Size of float: %zu\n", sizeof(float));
    printf("Size of double: %zu\n", sizeof(double));
    printf("Size of long double: %zu\n", sizeof(long double));
    printf("Size of intptr_t: %zu\n", sizeof(intptr_t));
    printf("Size of size_t: %zu\n", sizeof(size_t));
    printf("Size of ssize_t: %zu\n", sizeof(ssize_t));
    printf("Size of fpos_t: %zu\n", sizeof(fpos_t));
    printf("Size of off_t: %zu\n", sizeof(off_t));
    printf("Size of bool: %zu\n", sizeof(_Bool));
    printf("Size of pid_t: %zu\n", sizeof(pid_t));
    printf("Size of uid_t: %zu\n", sizeof(uid_t));
    printf("Size of gid_t: %zu\n", sizeof(gid_t));

    return EXIT_SUCCESS;
}

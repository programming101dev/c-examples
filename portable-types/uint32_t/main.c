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


#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>


int main(void)
{
    uint32_t value = 42;

    printf("Using PRIu32 (unsigned): %" PRIu32 "\n", value);
    printf("Using PRIx32 (hexadecimal): %" PRIx32 "\n", value);
    printf("Using PRIX32 (uppercase hexadecimal): %" PRIX32 "\n", value);

    return EXIT_SUCCESS;
}

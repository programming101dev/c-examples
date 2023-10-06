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

static void display_value(int16_t value);

int main(int argc, char *argv[])
{
    display_value(INT16_MIN);
    printf("----\n");
    display_value(INT16_MAX);

    return EXIT_SUCCESS;
}

static void display_value(int16_t value)
{
    printf("Using PRId16 (decimal): %" PRId16 "\n", value);
    printf("Using PRIi16 (decimal): %" PRIi16 "\n", value);
    printf("Using PRIu16 (unsigned): %" PRIu16 "\n", (uint16_t)value);
    printf("Using PRIx16 (hexadecimal): %" PRIx16 "\n", (uint16_t)value);
    printf("Using PRIX16 (uppercase hexadecimal): %" PRIX16 "\n", (uint16_t)value);
}

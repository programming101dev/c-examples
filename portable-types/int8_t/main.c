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

static void display_value(int8_t value);

int main(int argc, char *argv[])
{
    display_value(INT8_MIN);
    printf("----\n");
    display_value(INT8_MAX);

    return EXIT_SUCCESS;
}

static void display_value(int8_t value)
{
    printf("Using PRId8 (decimal): %" PRId8 "\n", value);
    printf("Using PRIi8 (decimal): %" PRIi8 "\n", value);
    printf("Using PRIu8 (unsigned): %" PRIu8 "\n", (uint8_t)value);
    printf("Using PRIx8 (hexadecimal): %" PRIx8 "\n", (uint8_t)value);
    printf("Using PRIX8 (uppercase hexadecimal): %" PRIX8 "\n", (uint8_t)value);
}

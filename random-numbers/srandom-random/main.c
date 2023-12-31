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
#include <time.h>

static void print_random_numbers(size_t count);

// TODO read this in off of the command line
#define COUNT 5

int main(void)
{
    unsigned int seed;

    // Set a seed value for random number generation based on the current time.
    seed = (unsigned int)time(NULL);
    srandom(seed);
    printf("First sequence\n");
    print_random_numbers(COUNT);

    // Set a different seed value for the second sequence
    srandom(seed + 1);
    printf("Second sequence\n");
    print_random_numbers(COUNT);

    // Same as the first sequence
    srandom(seed);
    printf("Third sequence\n");
    print_random_numbers(COUNT);

    return EXIT_SUCCESS;
}

static void print_random_numbers(size_t count)
{
    for(size_t i = 1; i <= count; i++)
    {
        long random_value;

        random_value = random();
        printf("Random number %zu: %ld\n", i, random_value);
    }
}

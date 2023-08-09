#include <stdio.h>
#include <stdlib.h>
#include <time.h>


static void print_random_numbers(size_t count);


int main(void)
{
    unsigned int seed;

    // Set a seed value for random number generation based on the current time.
    seed = time(NULL);
    srandom(seed);
    printf("First sequence\n");
    print_random_numbers(5);

    // Set a different seed value for the second sequence
    srandom(seed + 1);
    printf("Second sequence\n");
    print_random_numbers(5);

    // Same as the first sequence
    srandom(seed);
    printf("Third sequence\n");
    print_random_numbers(5);

    return EXIT_SUCCESS;
}

static void print_random_numbers(size_t count)
{
    for(size_t i = 1; i <= count; i++)
    {
        long randomValue = random();
        printf("Random number %zu: %ld\n", i, randomValue);
    }
}

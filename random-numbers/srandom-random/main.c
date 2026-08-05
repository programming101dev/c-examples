/*
 * Copyright 2026 D'Arcy Smith
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
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

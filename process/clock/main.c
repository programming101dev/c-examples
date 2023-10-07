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
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

static long long performCalculation(size_t size, size_t iterations);

#define MAX_NUMBER 100

int main(void)
{
    // TODO pass this in on the command line
    const size_t MAX_NUMBERS    = 100000;
    const size_t NUM_ITERATIONS = 20000;
    double       cpu_time_used;
    clock_t      start_time;
    pid_t        pid;

    pid        = fork();
    start_time = clock();

    if(pid < 0)
    {
        fprintf(stderr, "Fork failed\n");
        return EXIT_FAILURE;
    }

    if(pid == 0)
    {
        clock_t   end_time;
        long long sum;

        sum = performCalculation(MAX_NUMBERS, NUM_ITERATIONS);
        wait(NULL);
        end_time = clock();
        printf("Child: Sum of %zu random numbers for %zu iterations: %lld\n", MAX_NUMBERS, NUM_ITERATIONS, sum);
        printf("Child: Real Time: %jd clock ticks\n", (intmax_t)(end_time - start_time));
        cpu_time_used = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;
        printf("Child: CPU time used: %f seconds\n", cpu_time_used);
    }
    else
    {
        clock_t   end_time;
        long long sum;

        sum = performCalculation(MAX_NUMBERS, NUM_ITERATIONS);
        wait(NULL);
        end_time = clock();
        printf("Parent: Sum of %zu random numbers for %zu iterations: %lld\n", MAX_NUMBERS, NUM_ITERATIONS, sum);
        printf("Parent: Real Time: %jd clock ticks\n", (intmax_t)(end_time - start_time));
        cpu_time_used = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;
        printf("Parent: CPU time used: %f seconds\n", cpu_time_used);
    }

    return EXIT_SUCCESS;
}

static long long performCalculation(size_t size, size_t iterations)
{
    int      *numbers;
    long long sum;

    numbers = (int *)malloc(size * sizeof(int));

    if(numbers == NULL)
    {
        perror("Failed to allocate memory");
        exit(EXIT_FAILURE);
    }

    sum = 0;
    srand((unsigned int)time(NULL) ^ (unsigned int)getpid());

    for(size_t i = 0; i < size; i++)
    {
        numbers[i] = rand() % MAX_NUMBER;
    }

    for(size_t i = 0; i < iterations; i++)
    {
        for(size_t j = 0; j < size; j++)
        {
            sum += numbers[j];
        }
    }

    free(numbers);

    return sum;
}

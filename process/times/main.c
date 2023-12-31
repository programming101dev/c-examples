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
#include <sys/times.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

static long long performCalculation(size_t size, size_t iterations);
static void      printTimes(struct tms tms_data);

#define MAX_NUMBER 100

int main(void)
{
    // TODO pass this in on the command line
    const size_t MAX_NUMBERS    = 100000;
    const size_t NUM_ITERATIONS = 20000;
    pid_t        pid;

    pid = fork();

    if(pid < 0)
    {
        fprintf(stderr, "Fork failed\n");
        return EXIT_FAILURE;
    }

    if(pid == 0)
    {
        long long sum;

        sum = performCalculation(MAX_NUMBERS, NUM_ITERATIONS);
        printf("Sum of %zu random numbers for %zu iterations: %lld\n", MAX_NUMBERS, NUM_ITERATIONS, sum);
        exit(EXIT_SUCCESS);
    }
    else
    {
        struct tms start_tms;
        struct tms end_tms;
        long long  sum;

        times(&start_tms);
        sum = performCalculation(MAX_NUMBERS, NUM_ITERATIONS);
        wait(NULL);
        times(&end_tms);
        printf("Sum of %zu random numbers for %zu iterations: %lld\n", MAX_NUMBERS, NUM_ITERATIONS, sum);
        printf("Real Time: %jd clock ticks\n", (intmax_t)(end_tms.tms_stime - start_tms.tms_stime));
        times(&end_tms);
        printTimes(end_tms);
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

static void printTimes(struct tms tms_data)
{
    printf("User time: %jd clock ticks\n", (intmax_t)tms_data.tms_utime);
    printf("System time: %jd clock ticks\n", (intmax_t)tms_data.tms_stime);
    printf("Children's user time: %jd clock ticks\n", (intmax_t)tms_data.tms_cutime);
    printf("Children's system time: %jd clock ticks\n", (intmax_t)tms_data.tms_cstime);
}

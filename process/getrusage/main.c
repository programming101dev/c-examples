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
#include <sys/resource.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>


static long long performCalculation(size_t size, size_t iterations);
static void printUsage(struct rusage *ru);


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
        struct rusage start_ru;
        struct rusage end_ru;
        long long     sum;

        getrusage(RUSAGE_SELF, &start_ru);
        sum = performCalculation(MAX_NUMBERS, NUM_ITERATIONS);
        wait(NULL);
        getrusage(RUSAGE_CHILDREN, &end_ru);
        printf("Sum of %zu random numbers for %zu iterations: %lld\n", MAX_NUMBERS, NUM_ITERATIONS, sum);
        printUsage(&end_ru);
        getrusage(RUSAGE_SELF, &end_ru);
        printUsage(&end_ru);
    }

    return EXIT_SUCCESS;
}


static long long performCalculation(size_t size, size_t iterations)
{
    int       *numbers;
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


static void printUsage(struct rusage *ru)
{
    printf("User time: %jd.%06ld seconds\n", (intmax_t)ru->ru_utime
                                                         .tv_sec, (long)ru->ru_utime
                                                                          .tv_usec);
    printf("System time: %jd.%06ld seconds\n", (intmax_t)ru->ru_stime
                                                           .tv_sec, (long)ru->ru_stime
                                                                            .tv_usec);
    printf("Maximum resident set size (RSS): %ld kilobytes\n", ru->ru_maxrss);
    printf("Integral shared memory size: %ld kilobytes\n", ru->ru_ixrss);
    printf("Integral unshared data size: %ld kilobytes\n", ru->ru_idrss);
    printf("Integral unshared stack size: %ld kilobytes\n", ru->ru_isrss);
    printf("Page reclaims (soft page faults): %ld\n", ru->ru_minflt);
    printf("Page faults (hard page faults): %ld\n", ru->ru_majflt);
    printf("Swaps: %ld\n", ru->ru_nswap);
    printf("Block input operations: %ld\n", ru->ru_inblock);
    printf("Block output operations: %ld\n", ru->ru_oublock);
    printf("IPC messages sent: %ld\n", ru->ru_msgsnd);
    printf("IPC messages received: %ld\n", ru->ru_msgrcv);
    printf("Signals received: %ld\n", ru->ru_nsignals);
    printf("Voluntary context switches: %ld\n", ru->ru_nvcsw);
    printf("Involuntary context switches: %ld\n", ru->ru_nivcsw);
}

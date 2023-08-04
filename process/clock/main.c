#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <inttypes.h>

long long performCalculation(size_t size, size_t iterations);
void printTimes(clock_t start_time, clock_t end_time);


int main(void)
{
    const size_t MAX_NUMBERS = 100000;
    const size_t NUM_ITERATIONS = 20000;
    double cpu_time_used;

    pid_t pid = fork();

    if(pid < 0)
    {
        fprintf(stderr, "Fork failed\n");
        return EXIT_FAILURE;
    }
    else if(pid == 0)
    {
        long long sum;

        sum = performCalculation(MAX_NUMBERS, NUM_ITERATIONS);
        printf("Sum of %zu random numbers for %zu iterations: %lld\n", MAX_NUMBERS, NUM_ITERATIONS, sum);
        exit(EXIT_SUCCESS);
    }
    else
    {
        clock_t start_time, end_time;
        long long sum;

        start_time = clock();
        sum = performCalculation(MAX_NUMBERS, NUM_ITERATIONS);
        wait(NULL);
        end_time = clock();
        printf("Sum of %zu random numbers for %zu iterations: %lld\n", MAX_NUMBERS, NUM_ITERATIONS, sum);
        printf("Real Time: %jd clock ticks\n", (intmax_t)(end_time - start_time));
        cpu_time_used = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;
        printf("CPU time used: %f seconds\n", cpu_time_used);
    }

    return EXIT_SUCCESS;
}


long long performCalculation(size_t size, size_t iterations)
{
    int numbers[size];
    long long sum;

    srand(time(NULL) ^ getpid());

    for(int i = 0; i < size; i++)
    {
        numbers[i] = rand() % 100;
    }

    sum = 0;

    for(int i = 0; i < iterations; i++)
    {
        for (int j = 0; j < size; j++)
        {
            sum += numbers[j];
        }
    }

    return sum;
}

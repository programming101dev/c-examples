#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/times.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>


static long long performCalculation(size_t size, size_t iterations);
static void printTimes(struct tms tms_data);


int main(void)
{
    const size_t MAX_NUMBERS = 100000;
    const size_t NUM_ITERATIONS = 20000;

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
        struct tms start_tms;
        struct tms end_tms;
        long long sum;

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
    int *numbers;

    numbers = malloc(size * sizeof(int));

    if(numbers == NULL)
    {
        perror("Failed to allocate memory");
        exit(EXIT_FAILURE);
    }

    long long sum = 0;

    srand(time(NULL) ^ getpid());

    for(size_t i = 0; i < size; i++)
    {
        numbers[i] = rand() % 100;
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
    printf("User time: %jd clock ticks\n", (intmax_t) tms_data.tms_utime);
    printf("System time: %jd clock ticks\n", (intmax_t) tms_data.tms_stime);
    printf("Children's user time: %jd clock ticks\n", (intmax_t) tms_data.tms_cutime);
    printf("Children's system time: %jd clock ticks\n", (intmax_t) tms_data.tms_cstime);
}


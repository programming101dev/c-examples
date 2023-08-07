#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/resource.h>
#include <unistd.h>
#include <inttypes.h>

long long performCalculation(size_t size, size_t iterations);
void printUsage(struct rusage ru);

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
        struct rusage start_ru, end_ru;
        long long sum;

        getrusage(RUSAGE_SELF, &start_ru);
        sum = performCalculation(MAX_NUMBERS, NUM_ITERATIONS);
        wait(NULL);
        getrusage(RUSAGE_CHILDREN, &end_ru);
        printf("Sum of %zu random numbers for %zu iterations: %lld\n", MAX_NUMBERS, NUM_ITERATIONS, sum);
        printUsage(end_ru);
        getrusage(RUSAGE_SELF, &end_ru);
        printUsage(end_ru);
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
        for(int j = 0; j < size; j++)
        {
            sum += numbers[j];
        }
    }

    return sum;
}

void printUsage(struct rusage ru)
{
    printf("User time: %jd.%06ld seconds\n", (intmax_t)ru.ru_utime.tv_sec, (long)ru.ru_utime.tv_usec);
    printf("System time: %jd.%06ld seconds\n", (intmax_t)ru.ru_stime.tv_sec, (long)ru.ru_stime.tv_usec);
    printf("Maximum resident set size (RSS): %ld kilobytes\n", ru.ru_maxrss);
    printf("Integral shared memory size: %ld kilobytes\n", ru.ru_ixrss);
    printf("Integral unshared data size: %ld kilobytes\n", ru.ru_idrss);
    printf("Integral unshared stack size: %ld kilobytes\n", ru.ru_isrss);
    printf("Page reclaims (soft page faults): %ld\n", ru.ru_minflt);
    printf("Page faults (hard page faults): %ld\n", ru.ru_majflt);
    printf("Swaps: %ld\n", ru.ru_nswap);
    printf("Block input operations: %ld\n", ru.ru_inblock);
    printf("Block output operations: %ld\n", ru.ru_oublock);
    printf("IPC messages sent: %ld\n", ru.ru_msgsnd);
    printf("IPC messages received: %ld\n", ru.ru_msgrcv);
    printf("Signals received: %ld\n", ru.ru_nsignals);
    printf("Voluntary context switches: %ld\n", ru.ru_nvcsw);
    printf("Involuntary context switches: %ld\n", ru.ru_nivcsw);
}

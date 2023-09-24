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


#include <errno.h>
#include <getopt.h>
#include <inttypes.h>
#include <limits.h>
#include <pthread.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>


static void parse_arguments(int argc, char *argv[], char **main_seconds, char **thread_seconds);
static void handle_arguments(const char *binary_name, const char *main_seconds_str, const char *thread_seconds_str, time_t *main_seconds, unsigned int *thread_seconds);
time_t get_time_t_min(void) __attribute__((const));
time_t get_time_t_max(void) __attribute__((const));
time_t parse_time_t(const char *binary_name, time_t min, time_t max, const char *str);
static unsigned int parse_unsigned_int(const char *binary_name, const char *str);
_Noreturn static void usage(const char *program_name, int exit_code, const char *message);
static void *thread_function(void *arg);


#define UNKNOWN_OPTION_MESSAGE_LEN 24
#define BASE_TEN 10


// NOLINTBEGIN(cppcoreguidelines-avoid-non-const-global-variables)
static pthread_cond_t  cond_var = PTHREAD_COND_INITIALIZER;
static pthread_mutex_t mutex    = PTHREAD_MUTEX_INITIALIZER;
// NOLINTEND(cppcoreguidelines-avoid-non-const-global-variables)


int main(int argc, char *argv[])
{
    char            *main_seconds_str;
    char            *thread_seconds_str;
    time_t          main_seconds;
    unsigned int    thread_seconds;
    pthread_t       thread_id;
    struct timespec abs_timeout;
    int             result;
    main_seconds_str   = NULL;
    thread_seconds_str = NULL;
    parse_arguments(argc, argv, &main_seconds_str, &thread_seconds_str);
    handle_arguments(argv[0], main_seconds_str, thread_seconds_str, &main_seconds, &thread_seconds);

    // Create the thread
    pthread_create(&thread_id, NULL, thread_function, &thread_seconds);

    // Get the current time and set the timeout to 5 seconds
    clock_gettime(CLOCK_REALTIME, &abs_timeout);
    abs_timeout.tv_sec += main_seconds; // Changed to 5 seconds

    // Lock the mutex before waiting on the condition variable
#if defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wthread-safety-negative"
#endif
    pthread_mutex_lock(&mutex);
#if defined(__clang__)
#pragma clang diagnostic pop
#endif
    result = pthread_cond_timedwait(&cond_var, &mutex, &abs_timeout);
    pthread_mutex_unlock(&mutex);
    if(result == 0)
    {
        printf("Condition variable signaled before timeout.\n");
    }
    else if(result == ETIMEDOUT)
    {
        printf("Timeout occurred. Condition variable was not signaled.\n");
    }
    else
    {
        printf("Error while waiting for the condition variable.\n");
    }

    // Wait for the thread to finish
    pthread_join(thread_id, NULL);

    // Destroy mutex and condition variable
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond_var);
    return EXIT_SUCCESS;
}


static void parse_arguments(int argc, char *argv[], char **main_seconds, char **thread_seconds)
{
    int opt;
    opterr     = 0;
    while((opt = getopt(argc, argv, "hm:t:")) != -1)
    {
        switch(opt)
        {
            case 'm':
            {
                *main_seconds = optarg;
                break;
            }
            case 't':
            {
                *thread_seconds = optarg;
                break;
            }
            case 'h':
            {
                usage(argv[0], EXIT_SUCCESS, NULL);
            }
            case '?':
            {
                char message[UNKNOWN_OPTION_MESSAGE_LEN];
                snprintf(message, sizeof(message), "Unknown option '-%c'.", optopt);
                usage(argv[0], EXIT_FAILURE, message);
            }
            default:
            {
                usage(argv[0], EXIT_FAILURE, NULL);
            }
        }
    }
    if(optind != argc)
    {
        usage(argv[0], EXIT_FAILURE, "Too many arguments.");
    }
}


static void handle_arguments(const char *binary_name, const char *main_seconds_str, const char *thread_seconds_str, time_t *main_seconds, unsigned int *thread_seconds)
{
    time_t min;
    time_t max;
    if(main_seconds_str == NULL)
    {
        usage(binary_name, EXIT_FAILURE, "main seconds or nanoseconds are required.");
    }
    if(thread_seconds_str == NULL)
    {
        usage(binary_name, EXIT_FAILURE, "thread seconds or nanoseconds are required.");
    }
    min = get_time_t_min();
    max = get_time_t_max();
    *main_seconds   = parse_time_t(binary_name, min, max, main_seconds_str);
    *thread_seconds = parse_unsigned_int(binary_name, thread_seconds_str);
}


time_t get_time_t_min(void)
{
    time_t value;

    if(sizeof(time_t) == sizeof(char))
    {
        value = CHAR_MIN;
    }
    else if(sizeof(time_t) == sizeof(short))
    {
        value = SHRT_MIN;
    }
    else if(sizeof(time_t) == sizeof(int))
    {
        value = INT_MIN;
    }
    else if(sizeof(time_t) == sizeof(long))
    {
        value = LONG_MIN;
    }
    else if(sizeof(time_t) == sizeof(long long))
    {
        value = LLONG_MIN;
    }
    else
    {
        // Handle other sizes or display an error message
        fprintf(stderr, "Unsupported size of time_t\n");
        exit(EXIT_FAILURE);
    }

    return value;
}


time_t get_time_t_max(void)
{
    time_t value;

    if(sizeof(time_t) == sizeof(char))
    {
        value = CHAR_MAX;
    }
    else if(sizeof(time_t) == sizeof(short))
    {
        value = SHRT_MAX;
    }
    else if(sizeof(time_t) == sizeof(int))
    {
        value = INT_MAX;
    }
    else if(sizeof(time_t) == sizeof(long))
    {
        value = LONG_MAX;
    }
    else if(sizeof(time_t) == sizeof(long long))
    {
        value = LLONG_MAX;
    }
    else
    {
        fprintf(stderr, "Unsupported size of time_t\n");
        exit(EXIT_FAILURE);
    }

    return value;
}


time_t parse_time_t(const char *binary_name, time_t min, time_t max, const char *str)
{
    char     *endptr;
    intmax_t parsed_value;
    errno        = 0;
    parsed_value = strtoimax(str, &endptr, BASE_TEN);
    if(errno != 0)
    {
        usage(binary_name, EXIT_FAILURE, "Error parsing time.");
    }

    // Check if there are any non-numeric characters in the input string
    if(*endptr != '\0')
    {
        usage(binary_name, EXIT_FAILURE, "Invalid characters in input.");
    }
    if(parsed_value < min || parsed_value > max)
    {
        usage(binary_name, EXIT_FAILURE, "Unsigned integer out of range for time.");
    }
    return (time_t)parsed_value;
}


static unsigned int parse_unsigned_int(const char *binary_name, const char *str)
{
    char      *endptr;
    uintmax_t parsed_value;
    errno        = 0;
    parsed_value = strtoumax(str, &endptr, BASE_TEN);
    if(errno != 0)
    {
        usage(binary_name, EXIT_FAILURE, "Error parsing unsigned integer.");
    }

    // Check if there are any non-numeric characters in the input string
    if(*endptr != '\0')
    {
        usage(binary_name, EXIT_FAILURE, "Invalid characters in input.");
    }

    // Check if the parsed value is within the valid range for unsigned int
    if(parsed_value > UINT_MAX)
    {
        usage(binary_name, EXIT_FAILURE, "Unsigned integer out of range.");
    }
    return (unsigned int)parsed_value;
}


_Noreturn static void usage(const char *program_name, int exit_code, const char *message)
{
    if(message)
    {
        fprintf(stderr, "%s\n", message);
    }
    fprintf(stderr, "Usage: %s [-h] -m <seconds> -t <seconds>\n", program_name);
    fputs("Options:\n", stderr);
    fputs("  -h                Display this help message\n", stderr);
    fputs("  -m <seconds>      \n", stderr);
    fputs("  -t <seconds>  \n", stderr);
    exit(exit_code);
}


#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"


static void *thread_function(void *arg)
{
    unsigned sleep_time = *(unsigned int *)arg;
    printf("Thread will sleep for %u seconds.\n", sleep_time);
    sleep(sleep_time);

    // Signal the condition variable
#if defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wthread-safety-negative"
#endif
    pthread_mutex_lock(&mutex);
#if defined(__clang__)
#pragma clang diagnostic pop
#endif
    pthread_cond_signal(&cond_var);
    pthread_mutex_unlock(&mutex);
    return NULL;
}


#pragma GCC diagnostic pop

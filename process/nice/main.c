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

#include <errno.h>
#include <getopt.h>
#include <inttypes.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

static void           parse_arguments(int argc, char *argv[], char **increment);
static void           handle_arguments(const char *binary_name, const char *increment_str, int *increment);
static int            parse_int(const char *binary_name, const char *str);
_Noreturn static void usage(const char *program_name, int exit_code, const char *message);

#define UNKNOWN_OPTION_MESSAGE_LEN 24
#define BASE_TEN 10

int main(int argc, char *argv[])
{
    char *increment_str;
    int   increment;
    pid_t pid;
    int   current_priority;
    int   new_priority;

    parse_arguments(argc, argv, &increment_str);
    handle_arguments(argv[0], increment_str, &increment);
    pid              = getpid();
    current_priority = nice(0);
    printf("Process ID: %d\n", pid);
    printf("Current Priority: %d\n", current_priority);
    errno        = 0;
    new_priority = nice(increment);

    if(new_priority == -1 && errno != 0)
    {
        perror("nice");
        return EXIT_FAILURE;
    }

    printf("New Priority: %d\n", new_priority);

    return EXIT_SUCCESS;
}

static void parse_arguments(int argc, char *argv[], char **increment)
{
    int opt;

    opterr = 0;

    // Parse command-line options
    while((opt = getopt(argc, argv, "hs:")) != -1)
    {
        switch(opt)
        {
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

    if(optind >= argc)
    {
        usage(argv[0], EXIT_FAILURE, "The increment is required");
    }

    if(optind < argc - 1)
    {
        usage(argv[0], EXIT_FAILURE, "Too many arguments.");
    }

    *increment = argv[optind];
}

static void handle_arguments(const char *binary_name, const char *increment_str, int *increment)
{
    if(increment_str == NULL)
    {
        usage(binary_name, EXIT_FAILURE, "The increment is required.");
    }

    *increment = parse_int(binary_name, increment_str);
}

static int parse_int(const char *binary_name, const char *str)
{
    char    *endptr;
    intmax_t parsed_value;

    errno        = 0;
    parsed_value = strtoimax(str, &endptr, BASE_TEN);

    if(errno != 0)
    {
        usage(binary_name, EXIT_FAILURE, "Error parsing integer.");
    }

    // Check if there are any non-numeric characters in the input string
    if(*endptr != '\0')
    {
        usage(binary_name, EXIT_FAILURE, "Invalid characters in input.");
    }

    // Check if the parsed value is within the valid range for int
    if(parsed_value > INT_MAX || parsed_value < INT_MIN)
    {
        usage(binary_name, EXIT_FAILURE, "Integer out of range.");
    }

    return (int)parsed_value;
}

_Noreturn static void usage(const char *program_name, int exit_code, const char *message)
{
    if(message)
    {
        fprintf(stderr, "%s\n", message);
    }

    fprintf(stderr, "Usage: %s [-h] <increment>\n", program_name);
    fputs("Options:\n", stderr);
    fputs("  -h  Display this help message\n", stderr);
    exit(exit_code);
}

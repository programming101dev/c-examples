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

#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void           parse_arguments(int argc, char *argv[], char **string1, char **string2);
static void           handle_arguments(const char *binary_name, const char *string1, const char *string2);
_Noreturn static void usage(const char *program_name, int exit_code, const char *message);

#define UNKNOWN_OPTION_MESSAGE_LEN 24

int main(int argc, char *argv[])
{
    char *string1;
    char *string2;
    int   result;

    string1 = NULL;
    string2 = NULL;
    parse_arguments(argc, argv, &string1, &string2);
    handle_arguments(argv[0], string1, string2);
    result = strcmp(string1, string2);

    if(result == 0)
    {
        printf("\"%s\" and \"%s\" are identical.\n", string1, string2);
    }
    else
    {
        printf("\"%s\" and \"%s\" are different (%d).\n", string1, string2, result);
    }

    return EXIT_SUCCESS;
}

static void parse_arguments(int argc, char *argv[], char **string1, char **string2)
{
    int opt;

    opterr = 0;

    while((opt = getopt(argc, argv, "h")) != -1)
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
        usage(argv[0], EXIT_FAILURE, "The string1 and string2 are required");
    }

    if(optind + 1 >= argc)
    {
        usage(argv[0], EXIT_FAILURE, "The string2 is required");
    }

    if(optind < argc - 2)
    {
        usage(argv[0], EXIT_FAILURE, "Error: Too many arguments.");
    }

    *string1 = argv[optind];
    *string2 = argv[optind + 1];
}

static void handle_arguments(const char *binary_name, const char *string1, const char *string2)
{
    if(string1 == NULL)
    {
        usage(binary_name, EXIT_FAILURE, "string1 is required.");
    }

    if(string2 == NULL)
    {
        usage(binary_name, EXIT_FAILURE, "string2 is required.");
    }
}

_Noreturn static void usage(const char *program_name, int exit_code, const char *message)
{
    if(message)
    {
        fprintf(stderr, "%s\n", message);
    }

    fprintf(stderr, "Usage: %s [-h] <string1> <string2>\n", program_name);
    fputs("Options:\n", stderr);
    fputs("  -h  Display this help message\n", stderr);
    exit(exit_code);
}

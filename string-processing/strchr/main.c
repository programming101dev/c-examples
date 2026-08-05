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

#ifdef strchr
    #undef strchr
#endif

static void           parse_arguments(int argc, char *argv[], char **needle, char **haystack);
static void           handle_arguments(const char *binary_name, const char *needle_str, const char *haystack, char *needle);
_Noreturn static void usage(const char *program_name, int exit_code, const char *message);
static void           search_for(char needle, const char *haystack);

#define UNKNOWN_OPTION_MESSAGE_LEN 24

int main(int argc, char *argv[])
{
    char *needle_str;
    char *haystack;
    char  needle;

    needle_str = NULL;
    haystack   = NULL;

    parse_arguments(argc, argv, &needle_str, &haystack);
    handle_arguments(argv[0], needle_str, haystack, &needle);
    search_for(needle, haystack);

    return EXIT_SUCCESS;
}

static void parse_arguments(int argc, char *argv[], char **needle, char **haystack)
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

    if(optind + 1 >= argc)
    {
        usage(argv[0], EXIT_FAILURE, "Too few arguments.");
    }

    if(optind < argc - 2)
    {
        usage(argv[0], EXIT_FAILURE, "Too many arguments.");
    }

    *needle   = argv[optind];
    *haystack = argv[optind + 1];
}

static void handle_arguments(const char *binary_name, const char *needle_str, const char *haystack, char *needle)
{
    if(needle_str == NULL)
    {
        usage(binary_name, EXIT_FAILURE, "The needle is required.");
    }

    if(haystack == NULL)
    {
        usage(binary_name, EXIT_FAILURE, "The haystack is required.");
    }

    if(strlen(needle_str) != 1)
    {
        usage(binary_name, EXIT_FAILURE, "The needle must be a single character.");
    }

    *needle = needle_str[0];
}

_Noreturn static void usage(const char *program_name, int exit_code, const char *message)
{
    if(message)
    {
        fprintf(stderr, "%s\n", message);
    }

    fprintf(stderr, "Usage: %s [-h] <needle> <haystack>\n", program_name);
    fputs("Options:\n", stderr);
    fputs("  -h  Display this help message\n", stderr);
    exit(exit_code);
}

static void search_for(char needle, const char *haystack)
{
    const char *found_char;

    found_char = strchr(haystack, needle);

    if(found_char != NULL)
    {
        printf("Found character '%c' at position %td\n", needle, found_char - haystack);
    }
    else
    {
        printf("Character '%c' not found.\n", needle);
    }
}

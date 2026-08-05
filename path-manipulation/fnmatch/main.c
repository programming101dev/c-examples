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

#include <fnmatch.h>
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>

static void           parse_arguments(int argc, char *argv[], char **pattern);
static void           handle_arguments(const char *binary_name, const char *pattern);
_Noreturn static void usage(const char *program_name, int exit_code, const char *message);

#define UNKNOWN_OPTION_MESSAGE_LEN 24

int main(int argc, char *argv[])
{
    char *pattern;

    pattern = NULL;
    parse_arguments(argc, argv, &pattern);
    handle_arguments(argv[0], pattern);

    for(int i = optind + 1; i < argc; i++)
    {
        const char *path;

        path = argv[i];

        if(fnmatch(pattern, path, 0) != 0)
        {
            printf("Filename '%s' doesn't match the pattern.\n", path);
        }
        else
        {
            printf("Filename '%s' matches the pattern.\n", path);
        }
    }

    return EXIT_SUCCESS;
}

static void parse_arguments(int argc, char *argv[], char **pattern)
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
        usage(argv[0], EXIT_FAILURE, "The pattern and at least one path are required");
    }

    if(optind + 1 >= argc)
    {
        usage(argv[0], EXIT_FAILURE, "At least one path is required");
    }

    *pattern = argv[optind];
}

static void handle_arguments(const char *binary_name, const char *pattern)
{
    if(pattern == NULL)
    {
        usage(binary_name, EXIT_FAILURE, "The pattern is required.");
    }
}

_Noreturn static void usage(const char *program_name, int exit_code, const char *message)
{
    if(message)
    {
        fprintf(stderr, "%s\n", message);
    }

    fprintf(stderr, "Usage: %s [-h] <pattern> <path1> [path2 ...]\n", program_name);
    fputs("Options:\n", stderr);
    fputs("  -h  Display this help message\n", stderr);
    exit(exit_code);
}

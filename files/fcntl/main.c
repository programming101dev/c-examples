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
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

static void           parse_arguments(int argc, char *argv[], char **file_path);
static void           handle_arguments(const char *binary_name, const char *file_path);
_Noreturn static void usage(const char *program_name, int exit_code, const char *message);

#define UNKNOWN_OPTION_MESSAGE_LEN 24
#define LINE_LEN 1024

int main(int argc, char *argv[])
{
    char        *file_path;
    int          fd;
    char         buffer[LINE_LEN];
    ssize_t      bytesRead;
    unsigned int flags;

    file_path = NULL;
    parse_arguments(argc, argv, &file_path);
    handle_arguments(argv[0], file_path);
    fd = open(file_path, O_RDONLY | O_CLOEXEC);

    if(fd == -1)
    {
        perror("Error opening the file");
        return EXIT_FAILURE;
    }

    flags = (unsigned int)fcntl(fd, F_GETFL);

    if(flags == (unsigned int)-1)
    {
        perror("Error getting file descriptor flags");
        close(fd);
        return EXIT_FAILURE;
    }

    if(fcntl(fd, F_SETFL, (int)(flags | (unsigned int)O_NONBLOCK)) == -1)
    {
        perror("Error setting file descriptor to non-blocking mode");
        close(fd);
        return EXIT_FAILURE;
    }

    // Read from the file (will not block due to O_NONBLOCK)
    bytesRead = read(fd, buffer, sizeof(buffer) - 1);

    if(bytesRead == -1)
    {
        if(errno == EAGAIN)
        {
            printf("No data available (EAGAIN)\n");
        }
        else
        {
            perror("Error reading from the file");
            close(fd);
            return EXIT_FAILURE;
        }
    }
    else
    {
        buffer[bytesRead] = '\0';
        printf("Read %zd bytes:\n%s\n", bytesRead, buffer);
    }

    if(close(fd) == -1)
    {
        perror("Error closing the file");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

static void parse_arguments(int argc, char *argv[], char **file_path)
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
        usage(argv[0], EXIT_FAILURE, "The file path is required");
    }

    if(optind < argc - 1)
    {
        usage(argv[0], EXIT_FAILURE, "Too many arguments.");
    }

    *file_path = argv[optind];
}

static void handle_arguments(const char *binary_name, const char *file_path)
{
    if(file_path == NULL)
    {
        usage(binary_name, EXIT_FAILURE, "The file path is required.");
    }
}

_Noreturn static void usage(const char *program_name, int exit_code, const char *message)
{
    if(message)
    {
        fprintf(stderr, "%s\n", message);
    }

    fprintf(stderr, "Usage: %s [-h] <file path>\n", program_name);
    fputs("Options:\n", stderr);
    fputs("  -h  Display this help message\n", stderr);
    exit(exit_code);
}

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

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>

static void           parse_arguments(int argc, char *argv[], char **file_path);
static void           handle_arguments(const char *binary_name, const char *file_path);
_Noreturn static void usage(const char *program_name, int exit_code, const char *message);

#define UNKNOWN_OPTION_MESSAGE_LEN 24

int main(int argc, char *argv[])
{
    char       *file_path;
    int         fd;
    struct stat fileStat;

    file_path = NULL;
    parse_arguments(argc, argv, &file_path);
    handle_arguments(argv[0], file_path);
    fd = open(file_path, O_RDONLY | O_CLOEXEC);

    if(fd == -1)
    {
        perror("Error opening file");
        return EXIT_FAILURE;
    }

    if(fstat(fd, &fileStat) == -1)
    {
        perror("Error getting file stats");
        close(fd);
        return EXIT_FAILURE;
    }

    // Display file information
    printf("File Information:\n");
    printf("File Size: %lld bytes\n", (long long)fileStat.st_size);
    printf("File Permissions: %o\n", (unsigned int)fileStat.st_mode & (S_IRWXU | S_IRWXG | S_IRWXO));
    printf("File inode: %lld\n", (long long)fileStat.st_ino);
    printf("Device ID: %lld\n", (long long)fileStat.st_dev);
    printf("Number of hard links: %lld\n", (long long)fileStat.st_nlink);
    printf("File Owner UID: %u\n", fileStat.st_uid);
    printf("File Group GID: %u\n", fileStat.st_gid);
    printf("Last access time: %ld\n", fileStat.st_atime);
    printf("Last modification time: %ld\n", fileStat.st_mtime);
    printf("Last status change time: %ld\n", fileStat.st_ctime);

    if(close(fd) == -1)
    {
        perror("Error closing file descriptor");
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

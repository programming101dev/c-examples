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

#include <fcntl.h>
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

static void           parse_arguments(int argc, char *argv[], char **file_path);
static void           handle_arguments(const char *binary_name, const char *file_path);
_Noreturn static void usage(const char *program_name, int exit_code, const char *message);
static void           print_file_info(const struct stat *fileStat);
static void           print_special_type(const struct stat *fileStat);
static void           print_extended_type(const struct stat *fileStat);
static void           print_permissions(mode_t mode);
static void           print_time(const char *label, time_t timeValue);

#define UNKNOWN_OPTION_MESSAGE_LEN 24

int main(int argc, char *argv[])
{
    char       *file_path;
    struct stat fileStat;

    file_path = NULL;
    parse_arguments(argc, argv, &file_path);
    handle_arguments(argv[0], file_path);

    if(stat(file_path, &fileStat) == -1)
    {
        perror("Error getting file stats");
        return EXIT_FAILURE;
    }

    printf("File Information for '%s':\n\n", file_path);
    print_file_info(&fileStat);

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
        usage(argv[0], EXIT_FAILURE, "The file name is required");
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

    fprintf(stderr, "Usage: %s [-h] <file name>\n", program_name);
    fputs("Options:\n", stderr);
    fputs("  -h  Display this help message\n", stderr);
    exit(exit_code);
}

static void print_file_info(const struct stat *fileStat)
{
    print_special_type(fileStat);
    print_extended_type(fileStat);
    printf("File Size: %lld bytes\n", (long long)fileStat->st_size);
    print_permissions(fileStat->st_mode);
    printf("File inode: %lld\n", (long long)fileStat->st_ino);
    printf("Device ID: %lld\n", (long long)fileStat->st_dev);
    printf("Number of hard links: %lld\n", (long long)fileStat->st_nlink);
    printf("File Owner UID: %u\n", fileStat->st_uid);
    printf("File Group GID: %u\n", fileStat->st_gid);
    print_time("Last access time", fileStat->st_atime);
    print_time("Last modification time", fileStat->st_mtime);
    print_time("Last status change time", fileStat->st_ctime);
}

static void print_special_type(const struct stat *fileStat)
{
    if(S_ISBLK(fileStat->st_mode))
    {
        printf("Type: Block special file\n");
    }
    else if(S_ISCHR(fileStat->st_mode))
    {
        printf("Type: Character special file\n");
    }
    else if(S_ISDIR(fileStat->st_mode))
    {
        printf("Type: Directory\n");
    }
    else if(S_ISFIFO(fileStat->st_mode))
    {
        printf("Type: FIFO/Named Pipe\n");
    }
    else if(S_ISREG(fileStat->st_mode))
    {
        printf("Type: Regular file\n");
    }
    else if(S_ISLNK(fileStat->st_mode))
    {
        printf("Type: Symbolic link\n");
    }
    else if(S_ISSOCK(fileStat->st_mode))
    {
        printf("Type: Socket\n");
    }
    else
    {
        printf("Type: Unknown file type\n");
    }
}

static void print_extended_type(const struct stat *fileStat)
{
    (void)fileStat;    // Suppress unused parameter warning

#if defined(__cppcheck__)
    // Skip extended type checks when running Cppcheck
    printf("Extended type checks are skipped during Cppcheck analysis.\n");
#else
    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wundef"
    #if defined(S_TYPEISMQ) && S_TYPEISMQ != 0
    if(S_TYPEISMQ(fileStat))
    {
        printf("Type: Message Queue\n");
    }
    #endif
    #if defined(S_TYPEISSEM) && S_TYPEISSEM != 0
    if(S_TYPEISSEM(fileStat))
    {
        printf("Type: Semaphore\n");
    }
    #endif
    #if defined(S_TYPEISSHM) && S_TYPEISSHM != 0
    if(S_TYPEISSHM(fileStat))
    {
        printf("Type: Shared Memory\n");
    }
    #endif
    #pragma GCC diagnostic pop
#endif
}

static void print_permissions(mode_t mode)
{
    printf("File Permissions: %o\n", (unsigned int)mode & (S_IRWXU | S_IRWXG | S_IRWXO));
}

static void print_time(const char *label, time_t timeValue)
{
    printf("%s: %ld\n", label, timeValue);
}

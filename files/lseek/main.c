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
#include <fcntl.h>
#include <inttypes.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

static void           parse_arguments(int argc, char *argv[], char **file_path, char **offset);
static void           handle_arguments(const char *binary_name, const char *file_path, const char *offset_str, off_t *offset);
off_t                 get_off_t_min(void) __attribute__((const));
off_t                 get_off_t_max(void) __attribute__((const));
off_t                 parse_off_t(const char *binary_name, off_t min, off_t max, const char *str);
_Noreturn static void usage(const char *program_name, int exit_code, const char *message);
static void           display_file(int fd, const char *message, off_t offset);

#if defined(__APPLE__)
    #define D_OFF_FORMAT "%lld"
#else
    #define D_OFF_FORMAT "%ld"
#endif
#define UNKNOWN_OPTION_MESSAGE_LEN 24
#define BASE_TEN 10

int main(int argc, char *argv[])
{
    char *file_path;
    char *offset_str;
    off_t offset;
    int   fd;

    file_path  = NULL;
    offset_str = NULL;
    parse_arguments(argc, argv, &file_path, &offset_str);
    handle_arguments(argv[0], file_path, offset_str, &offset);
    fd = open(file_path, O_RDONLY | O_CLOEXEC);

    if(fd == -1)
    {
        perror("Error opening the file");
        return EXIT_FAILURE;
    }

    display_file(fd, "File contents", 0);
    lseek(fd, 0L, SEEK_SET);
    display_file(fd, "\n\nFile contents after SEEK_SET", 0);
    lseek(fd, 0L, SEEK_SET);
    lseek(fd, offset, SEEK_CUR);
    display_file(fd, "\n\nFile contents after SEEK_CUR", offset);
    close(fd);

    return EXIT_SUCCESS;
}

static void parse_arguments(int argc, char *argv[], char **file_path, char **offset)
{
    int opt;

    opterr = 0;

    while((opt = getopt(argc, argv, "ho:")) != -1)
    {
        switch(opt)
        {
            case 'o':
            {
                *offset = optarg;
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

static void handle_arguments(const char *binary_name, const char *file_path, const char *offset_str, off_t *offset)
{
    off_t min;
    off_t max;

    if(file_path == NULL)
    {
        usage(binary_name, EXIT_FAILURE, "The file path is required.");
    }

    if(offset_str == NULL)
    {
        usage(binary_name, EXIT_FAILURE, "The offset is required.");
    }

    min     = get_off_t_min();
    max     = get_off_t_max();
    *offset = parse_off_t(binary_name, min, max, offset_str);
}

off_t get_off_t_min(void)
{
    off_t value;

    if(sizeof(off_t) == sizeof(char))
    {
        value = SCHAR_MIN;
    }
    else if(sizeof(off_t) == sizeof(short))
    {
        value = SHRT_MIN;
    }
    else if(sizeof(off_t) == sizeof(int))
    {
        value = INT_MIN;
    }
    else if(sizeof(off_t) == sizeof(long))
    {
        value = LONG_MIN;
    }
    else if(sizeof(off_t) == sizeof(long long))
    {
        value = LLONG_MIN;
    }
    else
    {
        // Handle other sizes or display an error message
        fprintf(stderr, "Unsupported size of off_t\n");
        exit(EXIT_FAILURE);
    }

    return value;
}

off_t get_off_t_max(void)
{
    off_t value;

    if(sizeof(off_t) == sizeof(char))
    {
        value = SCHAR_MAX;
    }
    else if(sizeof(off_t) == sizeof(short))
    {
        value = SHRT_MAX;
    }
    else if(sizeof(off_t) == sizeof(int))
    {
        value = INT_MAX;
    }
    else if(sizeof(off_t) == sizeof(long))
    {
        value = LONG_MAX;
    }
    else if(sizeof(off_t) == sizeof(long long))
    {
        value = LLONG_MAX;
    }
    else
    {
        fprintf(stderr, "Unsupported size of off_t\n");
        exit(EXIT_FAILURE);
    }

    return value;
}

off_t parse_off_t(const char *binary_name, off_t min, off_t max, const char *str)
{
    char    *endptr;
    intmax_t parsed_value;

    errno        = 0;
    parsed_value = strtoimax(str, &endptr, BASE_TEN);

    if(errno != 0)
    {
        usage(binary_name, EXIT_FAILURE, "Error parsing off_t.");
    }

    // Check if there are any non-numeric characters in the input string
    if(*endptr != '\0')
    {
        usage(binary_name, EXIT_FAILURE, "Invalid characters in input.");
    }

    if(parsed_value < min || parsed_value > max)
    {
        usage(binary_name, EXIT_FAILURE, "off_t value out of range.");
    }

    return (off_t)parsed_value;
}

_Noreturn static void usage(const char *program_name, int exit_code, const char *message)
{
    if(message)
    {
        fprintf(stderr, "%s\n", message);
    }

    fprintf(stderr, "Usage: %s [-h] <file path>\n", program_name);
    fputs("Options:\n", stderr);
    fputs("  -h           Display this help message\n", stderr);
    fputs("  -o <offset>  The offset to move from the start of the file\n", stderr);
    exit(exit_code);
}

static void display_file(int fd, const char *message, off_t offset)
{
    char ch;
    printf("%s " D_OFF_FORMAT ":\n\n", message, offset);

    while(read(fd, &ch, 1) > 0)
    {
        write(STDOUT_FILENO, &ch, 1);
    }
}

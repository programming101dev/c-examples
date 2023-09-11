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
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


static void parse_arguments(int argc, char *argv[], char **file_path, char **offset);
static void handle_arguments(const char *binary_name, const char *file_path, const char *offset_str, off_t *offset);
static off_t parse_offset(const char *binary_name, const char *offset_str);
_Noreturn static void usage(const char *program_name, int exit_code, const char *message);
static void display_file(int fd, const char *message, off_t offset);


#if defined(__APPLE__)
#define D_OFF_FORMAT "%lld"
#else
#define D_OFF_FORMAT "%ld"
#endif


int main(int argc, char *argv[])
{
    char *file_path;
    char *offset_str;
    off_t offset;
    int fd;

    file_path = NULL;
    offset_str = NULL;
    parse_arguments(argc, argv, &file_path, &offset_str);
    handle_arguments(argv[0], file_path, offset_str, &offset);
    fd = open(file_path, O_RDONLY);

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
                char message[24];

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
        usage(argv[0], EXIT_FAILURE, "The group id is required");
    }

    if(optind < argc - 1)
    {
        usage(argv[0], EXIT_FAILURE, "Too many arguments.");
    }

    *file_path = argv[optind];
}


static void handle_arguments(const char *binary_name, const char *file_path, const char *offset_str, off_t *offset)
{
    if(file_path == NULL)
    {
        usage(binary_name, EXIT_FAILURE, "The file path is required.");
    }

    if(offset_str == NULL)
    {
        usage(binary_name, EXIT_FAILURE, "The offset is required.");
    }

    *offset = parse_offset(binary_name, offset_str);
}


static off_t parse_offset(const char *binary_name, const char *offset_str)
{
    char *endptr;
    long long int parsed_offset;

    errno = 0;
    parsed_offset = strtoll(offset_str, &endptr, 10);

    if(errno != 0)
    {
        usage(binary_name, EXIT_FAILURE, "Error parsing offset.");
    }

    // Check if there are any non-numeric characters in offset_str
    if(*endptr != '\0')
    {
        usage(binary_name, EXIT_FAILURE, "Invalid characters in offset.");
    }

    // Check if the offset is within the valid range
    if(parsed_offset < LONG_MIN || parsed_offset > LONG_MAX)
    {
        usage(binary_name, EXIT_FAILURE, "Offset out of range.");
    }

    return (off_t)parsed_offset;
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

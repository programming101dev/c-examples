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
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

static void           parse_arguments(int argc, char *argv[], char **file_path);
static void           handle_arguments(const char *binary_name, const char *file_path);
_Noreturn static void usage(const char *program_name, int exit_code, const char *message);

#define FIFO_FILE "./fifo_example"
#define UNKNOWN_OPTION_MESSAGE_LEN 24
#define LINE_LEN 1024

int main(int argc, char *argv[])
{
    char *file_path;
    int   fd;
    FILE *file;
    char  line[LINE_LEN];
    char *saveptr;

    file_path = NULL;
    parse_arguments(argc, argv, &file_path);
    handle_arguments(argv[0], file_path);
    fd = open(FIFO_FILE, O_WRONLY | O_CLOEXEC);

    if(fd == -1)
    {
        perror("open");
        exit(EXIT_FAILURE);
    }

    file = fopen(file_path, "re");

    if(file == NULL)
    {
        perror("fopen");
        close(fd);
        exit(EXIT_FAILURE);
    }

    while(fgets(line, sizeof(line), file) != NULL)
    {
        const char *word;

        word = strtok_r(line, " \t\n", &saveptr);

        while(word != NULL)
        {
            size_t  word_len;
            uint8_t size;

            word_len = strlen(word);

            if(word_len > UINT8_MAX)
            {
                fprintf(stderr, "Word exceeds maximum length\n");
                fclose(file);
                close(fd);
                exit(EXIT_FAILURE);
            }

            // Write the size of the word as uint8_t
            size = (uint8_t)word_len;
            write(fd, &size, sizeof(uint8_t));

            // Write the word
            write(fd, word, word_len);
            word = strtok_r(NULL, " \t\n", &saveptr);
        }
    }

    fclose(file);
    close(fd);

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

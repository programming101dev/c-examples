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

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

static void           parse_arguments(int argc, char *argv[], char **file_path);
static void           handle_arguments(const char *binary_name, const char *file_path);
_Noreturn static void usage(const char *program_name, int exit_code, const char *message);
static void           child_process(int sockfd, const char *file_path);
static void           parent_process(int sockfd);
static void           send_word(int sockfd, const char *word, uint8_t length);
_Noreturn static void error_exit(const char *msg);

#define MAX_WORD_LENGTH 255
#define UNKNOWN_OPTION_MESSAGE_LEN 24

int main(int argc, char *argv[])
{
    char *file_path;
    int   sockfd[2];
    pid_t pid;

    file_path = NULL;
    parse_arguments(argc, argv, &file_path);
    handle_arguments(argv[0], file_path);

    if(socketpair(AF_UNIX, SOCK_STREAM, 0, sockfd) == -1)
    {
        error_exit("Error creating socket pair");
    }

    pid = fork();

    if(pid == -1)
    {
        error_exit("Error creating child process");
    }

    if(pid == 0)
    {
        close(sockfd[1]);
        child_process(sockfd[0], file_path);
    }
    else
    {
        close(sockfd[0]);
        parent_process(sockfd[1]);
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

static void send_word(int sockfd, const char *word, uint8_t length)
{
    ssize_t written_bytes;

    printf("Child: sending word of length %u: %s\n", length, word);
    written_bytes = write(sockfd, &length, sizeof(length));

    if(written_bytes < 0)
    {
        error_exit("Error writing word length to socket");
    }

    if(length > 0)
    {
        written_bytes = write(sockfd, word, length);

        if(written_bytes < 0)
        {
            error_exit("Error writing word to socket");
        }
    }
}

_Noreturn static void error_exit(const char *msg)
{
    perror(msg);
    exit(EXIT_FAILURE);
}

static void child_process(int sockfd, const char *file_path)
{
    FILE   *file;
    int     ch;
    char    word[MAX_WORD_LENGTH];
    uint8_t length;

    file = fopen(file_path, "re");

    if(file == NULL)
    {
        error_exit("Error opening file");
    }

    length = 0;

    while((ch = fgetc(file)) != EOF)
    {
        if(ch == ' ' || ch == '\n' || ch == '\t')
        {
            if(length > 0)
            {
                word[length] = '\0';
                send_word(sockfd, word, length);
                length = 0;
            }
        }
        else
        {
            if(length >= MAX_WORD_LENGTH)
            {
                error_exit("Encountered a word longer than the maximum allowed length");
            }

            word[length++] = (char)ch;
        }
    }

    if(length > 0)
    {
        word[length] = '\0';
        send_word(sockfd, word, length);
    }

    send_word(sockfd, NULL, 0);

    if(fclose(file) != 0)
    {
        error_exit("Error closing file");
    }

    close(sockfd);
}

static void parent_process(int sockfd)
{
    while(1)
    {
        uint8_t length;
        ssize_t read_bytes;
        char    word[MAX_WORD_LENGTH];

        read_bytes = read(sockfd, &length, sizeof(length));

        if(read_bytes < 0)
        {
            error_exit("Error reading word length from socket");
        }

        if(length == 0)
        {
            break;
        }

        read_bytes = read(sockfd, word, length);

        if(read_bytes < 0)
        {
            error_exit("Error reading word from socket");
        }

        word[length] = '\0';
        printf("Parent: received word of length %u: %s\n", length, word);
    }

    close(sockfd);
}

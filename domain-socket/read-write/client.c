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


#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>


static void parse_arguments(int argc, char *argv[], char **file_path);
static void handle_arguments(const char *binary_name, const char *file_path);
_Noreturn static void usage(const char *program_name, int exit_code, const char *message);


#define SOCKET_PATH "/tmp/example_socket"


int main(int argc, char *argv[])
{
    char *file_path;
    int sockfd;
    struct sockaddr_un server_addr;

    file_path = NULL;
    parse_arguments(argc, argv, &file_path);
    handle_arguments(argv[0], file_path);
    sockfd = socket(AF_UNIX, SOCK_STREAM, 0);

    if(sockfd == -1)
    {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    memset(&server_addr, 0, sizeof(struct sockaddr_un));
    server_addr.sun_family = AF_UNIX;
    strcpy(server_addr.sun_path, SOCKET_PATH);

    if(connect(sockfd, (struct sockaddr *) &server_addr, sizeof(struct sockaddr_un)) == -1)
    {
        perror("connect");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    FILE *file = fopen(file_path, "r");

    if(file == NULL)
    {
        perror("fopen");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    // Read and parse words from the file
    char line[1024]; // Adjust the buffer size as needed
    while(fgets(line, sizeof(line), file) != NULL)
    {
        char *word;
        word = strtok(line, " \t\n");
        while(word != NULL)
        {
            size_t word_len = strlen(word);
            if(word_len > UINT8_MAX)
            {
                fprintf(stderr, "Word exceeds maximum length\n");
                fclose(file);
                close(sockfd);
                exit(EXIT_FAILURE);
            }

            // Write the size of the word as uint8_t
            uint8_t size = (uint8_t) word_len;
            send(sockfd, &size, sizeof(uint8_t), 0);

            // Write the word
            write(sockfd, word, word_len);

            word = strtok(NULL, " \t\n");
        }
    }

    fclose(file);
    close(sockfd);
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


static void handle_arguments(const char *binary_name, const char *file_name)
{
    if(file_name == NULL)
    {
        usage(binary_name, EXIT_FAILURE, "");
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

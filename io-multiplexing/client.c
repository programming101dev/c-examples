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
#include <sys/un.h>
#include <time.h>
#include <unistd.h>


static void parse_arguments(int argc, char *argv[], char **file_path);
static void handle_arguments(const char *binary_name, const char *file_path);
_Noreturn static void usage(const char *program_name, int exit_code, const char *message);
static void send_word(int sockfd, const char *word, uint8_t length);
_Noreturn static void error_exit(const char *msg);
static int connect_to_server(const char *path);
static int socket_create(void);
static void setup_socket_address(struct sockaddr_un *addr, const char *path);
static void socket_close(int sockfd);


#define SOCKET_PATH "/tmp/example_socket"
#define UNKNOWN_OPTION_MESSAGE_LEN 24
#define LINE_LEN 1024
#define MILLISECONDS_IN_NANOSECONDS 1000000
#define MIN_DELAY_MILLISECONDS 500
#define MAX_ADDITIONAL_NANOSECONDS 1000000000


// TODO: fork N children - make N a command line argument


int main(int argc, char *argv[])
{
    char *file_path;
    FILE *file;
    int  sockfd;
    char line[LINE_LEN];

    file_path = NULL;
    parse_arguments(argc, argv, &file_path);
    handle_arguments(argv[0], file_path);
    file = fopen(file_path, "re");

    if(file == NULL)
    {
        perror("fopen");
        exit(EXIT_FAILURE);
    }

    sockfd = connect_to_server(SOCKET_PATH);

    while(fgets(line, sizeof(line), file) != NULL)
    {
        char *word;

        word = strtok(line, " \t\n");

        while(word != NULL)
        {
            uint8_t size;
            size_t  word_len = strlen(word);
            if(word_len > UINT8_MAX)
            {
                fprintf(stderr, "Word exceeds maximum length\n");
                fclose(file);
                close(sockfd);
                exit(EXIT_FAILURE);
            }

            size             = (uint8_t)word_len;
            send_word(sockfd, word, size);
            word = strtok(NULL, " \t\n");
        }
    }

    fclose(file);
    socket_close(sockfd);

    return EXIT_SUCCESS;
}


static void parse_arguments(int argc, char *argv[], char **file_path)
{
    int opt;
    opterr     = 0;
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
        usage(argv[0], EXIT_FAILURE, "The group id is required");
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
    ssize_t         written_bytes;
    struct timespec delay;
    printf("Client: sending word of length %u: %s\n", length, word);
    written_bytes = send(sockfd, &length, sizeof(uint8_t), 0);
    if(written_bytes < 0)
    {
        error_exit("Error writing word length to socket");
    }
    if(length > 0)
    {
        written_bytes = send(sockfd, word, length, 0);
        if(written_bytes < 0)
        {
            error_exit("Error writing word to socket");
        }
    }

    // Add random delay between 500ms and 1500ms
    delay.tv_sec  = 0;
    delay.tv_nsec = MIN_DELAY_MILLISECONDS * MILLISECONDS_IN_NANOSECONDS + (rand() % MAX_ADDITIONAL_NANOSECONDS);
    nanosleep(&delay, NULL);
}


_Noreturn static void error_exit(const char *msg)
{
    perror(msg);
    exit(EXIT_FAILURE);
}


static int connect_to_server(const char *path)
{
    int                sockfd;
    struct sockaddr_un addr;
    sockfd = socket_create();
    setup_socket_address(&addr, path);
    if(connect(sockfd, (struct sockaddr *)&addr, sizeof(addr)) == -1)
    {
        perror("Connection failed");
        close(sockfd);
        exit(EXIT_FAILURE);
    }
    printf("Connected to %s\n", path);
    return sockfd;
}


static int socket_create(void)
{
    int sockfd;
    sockfd = socket(AF_UNIX, SOCK_STREAM, 0);
    if(sockfd == -1)
    {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }
    return sockfd;
}


static void setup_socket_address(struct sockaddr_un *addr, const char *path)
{
    memset(addr, 0, sizeof(*addr));
    addr->sun_family = AF_UNIX;
    strncpy(addr->sun_path, path, sizeof(addr->sun_path) - 1);
    addr->sun_path[sizeof(addr->sun_path) - 1] = '\0';
}


static void socket_close(int sockfd)
{
    if(close(sockfd) == -1)
    {
        perror("Error closing socket");
        exit(EXIT_FAILURE);
    }
}

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
#include <unistd.h>


static void parse_arguments(int argc, char *argv[], char **file_path);
static void handle_arguments(const char *binary_name, const char *file_path);
_Noreturn static void usage(const char *program_name, int exit_code, const char *message);
static int socket_create(void);
static void setup_socket_address(struct sockaddr_storage *addr, const char *path);
static int connect_to_server(int sockfd, struct sockaddr_storage *addr);
static void socket_close(int sockfd);


#define SOCKET_PATH "/tmp/example_socket"
#define LINE_LEN 1024
#define UNKNOWN_OPTION_MESSAGE_LEN 24


int main(int argc, char *argv[])
{
    char                    *file_path;
    FILE                    *file;
    struct sockaddr_storage addr;
    int                     sockfd;
    char                    line[LINE_LEN];

    file_path = NULL;
    parse_arguments(argc, argv, &file_path);
    handle_arguments(argv[0], file_path);
    file = fopen(file_path, "re");

    if(file == NULL)
    {
        perror("fopen");
        exit(EXIT_FAILURE);
    }

    setup_socket_address(&addr, SOCKET_PATH);
    sockfd = socket_create();
    connect_to_server(sockfd, &addr);

    while(fgets(line, sizeof(line), file) != NULL)
    {
        char *word;

        word = strtok(line, " \t\n");

        while(word != NULL)
        {
            size_t  word_len;
            uint8_t size;

            word_len = strlen(word);

            if(word_len > UINT8_MAX)
            {
                fprintf(stderr, "Word exceeds maximum length\n");
                fclose(file);
                close(sockfd);
                exit(EXIT_FAILURE);
            }

            // Write the size of the word as uint8_t
            size             = (uint8_t)word_len;
            send(sockfd, &size, sizeof(uint8_t), 0);

            // Write the word
            send(sockfd, word, word_len, 0);
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
        usage(binary_name, EXIT_FAILURE, "The file name is required.");
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


static int connect_to_server(int sockfd, struct sockaddr_storage *addr)
{
    if(connect(sockfd, (struct sockaddr *)addr, sizeof(*addr)) == -1)
    {
        perror("Connection failed");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

//    printf("Connected to %s\n", path);

    return sockfd;
}


static int socket_create(void)
{
    int sockfd;

#ifdef SOCK_CLOEXEC
    sockfd = socket(AF_UNIX, SOCK_STREAM | SOCK_CLOEXEC, 0);
#else
    sockfd = socket(AF_UNIX, SOCK_STREAM, 0);   // NOLINT(android-cloexec-socket)
#endif

    if(sockfd == -1)
    {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    return sockfd;
}


static void setup_socket_address(struct sockaddr_storage *addr, const char *path)
{
    struct sockaddr_un *addr_un;

    memset(addr, 0, sizeof(*addr));
    addr_un = (struct sockaddr_un *)addr;
    addr_un->sun_family = AF_UNIX;
    strncpy(addr_un->sun_path, path, sizeof(addr_un->sun_path) - 1);
    addr_un->sun_path[sizeof(addr_un->sun_path) - 1] = '\0';
}


static void socket_close(int sockfd)
{
    if(close(sockfd) == -1)
    {
        perror("Error closing socket");
        exit(EXIT_FAILURE);
    }
}

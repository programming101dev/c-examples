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
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>


static void parse_arguments(int argc, char *argv[], char **address_family_str, char **socket_type_str);
static int string_to_address_family(const char *address_family_str) __attribute__((pure));
static int string_to_socket_type(const char *socket_type_str) __attribute__((pure));
static void handle_arguments(const char *binary_name, const char *address_family_str, const char *socket_type_str, int *address_family, int *socket_type);
_Noreturn static void usage(const char *program_name, int exit_code, const char *message);
static int socket_create(int domain, int type, int protocol);
static void socket_close(int sock_fd);


int main(int argc, char *argv[])
{
    char *address_family_str;
    char *socket_type_str;
    int address_family;
    int socket_type;
    int sockfd;

    address_family_str = NULL;
    socket_type_str = NULL;
    parse_arguments(argc, argv, &address_family_str, &socket_type_str);
    handle_arguments(argv[0], address_family_str, socket_type_str, &address_family, &socket_type);
    sockfd = socket_create(address_family, socket_type, 0);
    printf("Socket created\n");
    socket_close(sockfd);
    printf("Socket closed\n");

    return EXIT_SUCCESS;
}


static void parse_arguments(int argc, char *argv[], char **address_family_str, char **socket_type_str)
{
    int opt;

    opterr = 0;

    while ((opt = getopt(argc, argv, "h")) != -1)
    {
        switch (opt)
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

    if (optind + 1 >= argc)
    {
        usage(argv[0], EXIT_FAILURE, "Too few arguments.");
    }

    if (optind < argc - 2)
    {
        usage(argv[0], EXIT_FAILURE, "Too many arguments.");
    }

    *address_family_str = argv[optind];
    *socket_type_str = argv[optind + 1];
}


static void handle_arguments(const char *binary_name, const char *address_family_str, const char *socket_type_str, int *address_family, int *socket_type)
{
    if (address_family_str == NULL)
    {
        usage(binary_name, EXIT_FAILURE, "The address family is required.");
    }

    if (socket_type_str == NULL)
    {
        usage(binary_name, EXIT_FAILURE, "The socket type is required.");
    }

    *address_family = string_to_address_family(address_family_str);
    *socket_type = string_to_socket_type(socket_type_str);
}


_Noreturn static void usage(const char *program_name, int exit_code, const char *message)
{
    if (message)
    {
        fprintf(stderr, "%s\n", message);
    }

    fprintf(stderr, "Usage: %s [-h] <address_family> <socket_type>\n", program_name);
    fputs("Options:\n", stderr);
    fputs("  -h  Display this help message\n", stderr);
    exit(exit_code);
}


static int string_to_address_family(const char *address_family_str)
{
    if(address_family_str == NULL)
    {
        usage("Invalid address family string.", EXIT_FAILURE, NULL);
    }

    if (strcmp(address_family_str, "AF_INET") == 0)
    {
        return AF_INET;
    }
    else if (strcmp(address_family_str, "AF_INET6") == 0)
    {
        return AF_INET6;
    }
    else if (strcmp(address_family_str, "AF_UNIX") == 0)
    {
        return AF_UNIX;
    }
    else
    {
        usage("Invalid address family string.", EXIT_FAILURE, NULL);
    }
}

static int string_to_socket_type(const char *socket_type_str)
{
    if(socket_type_str == NULL)
    {
        usage("Invalid socket type string.", EXIT_FAILURE, NULL);
    }

    if (strcmp(socket_type_str, "SOCK_STREAM") == 0)
    {
        return SOCK_STREAM;
    }
    else if (strcmp(socket_type_str, "SOCK_DGRAM") == 0)
    {
        return SOCK_DGRAM;
    }
    else if (strcmp(socket_type_str, "SOCK_RAW") == 0)
    {
        return SOCK_RAW;
    }
    else if (strcmp(socket_type_str, "SOCK_SEQPACKET") == 0)
    {
        return SOCK_SEQPACKET;
    }
    else
    {
        usage("Invalid socket type string.", EXIT_FAILURE, NULL);
    }
}


static int socket_create(int domain, int type, int protocol)
{
    int sockfd;

    sockfd = socket(domain, type, protocol);

    if(sockfd == -1)
    {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    return sockfd;
}


static void socket_close(int client_fd)
{
    if (close(client_fd) == -1)
    {
        perror("Error closing socket");
        exit(EXIT_FAILURE);
    }
}

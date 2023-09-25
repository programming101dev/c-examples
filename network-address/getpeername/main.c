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


#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>


static void parse_arguments(int argc, char *argv[], char **server_address, char **service);
static void handle_arguments(const char *binary_name, const char *server_address, const char *service);
_Noreturn static void usage(const char *program_name, int exit_code, const char *message);


#define UNKNOWN_OPTION_MESSAGE_LEN 24


int main(int argc, char *argv[])
{
    char               *server_address;
    char               *service;
    int                sockfd;
    struct addrinfo    hints;
    struct addrinfo    *result;
    struct addrinfo    *rp;
    int                status;
    struct sockaddr_in peer_addr;
    socklen_t          peer_addr_len;
    char               ipstr[INET_ADDRSTRLEN];

    server_address = NULL;
    service        = NULL;
    parse_arguments(argc, argv, &server_address, &service);
    handle_arguments(argv[0], server_address, service);
    // TODO: this should be AF_INET or AF_INET6
#ifdef SOCK_CLOEXEC
    sockfd = socket(AF_INET, SOCK_STREAM | SOCK_CLOEXEC, 0);
#else
    sockfd = socket(AF_INET, SOCK_STREAM, 0);   // NOLINT(android-cloexec-socket)
#endif

    if(sockfd == -1)
    {
        perror("socket");
        return EXIT_FAILURE;
    }

    memset(&hints, 0, sizeof(hints));
    hints.ai_family   = AF_UNSPEC;  // Allow both IPv4 and IPv6
    hints.ai_socktype = SOCK_STREAM;
    status = getaddrinfo(server_address, service, &hints, &result);

    if(status != 0)
    {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
        close(sockfd);
        return EXIT_FAILURE;
    }

    // Attempt to connect to the server using the available address info
    for(rp = result; rp != NULL; rp = rp->ai_next)
    {
        if(connect(sockfd, rp->ai_addr, rp->ai_addrlen) == 0)
        {
            break; // Connected successfully
        }
    }

    if(rp == NULL)
    {
        perror("connect");
        close(sockfd);
        freeaddrinfo(result);
        return EXIT_FAILURE;
    }

    // Get the peer address and port number associated with the socket
    peer_addr_len = sizeof(peer_addr);

    if(getpeername(sockfd, (struct sockaddr *)&peer_addr, &peer_addr_len) == -1)
    {
        perror("getpeername");
        close(sockfd);
        freeaddrinfo(result);
        return EXIT_FAILURE;
    }

    inet_ntop(AF_INET, &(peer_addr.sin_addr), ipstr, INET_ADDRSTRLEN);
    printf("Connected to: %s:%s\n", ipstr, service);
    close(sockfd);
    freeaddrinfo(result);

    return EXIT_SUCCESS;
}


static void parse_arguments(int argc, char *argv[], char **server_address, char **service)
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

    if(optind + 1 >= argc)
    {
        usage(argv[0], EXIT_FAILURE, "Too few arguments.");
    }

    if(optind < argc - 2)
    {
        usage(argv[0], EXIT_FAILURE, "Too many arguments.");
    }

    *server_address = argv[optind];
    *service        = argv[optind + 1];
}


static void handle_arguments(const char *binary_name, const char *server_address, const char *service)
{
    if(server_address == NULL)
    {
        usage(binary_name, EXIT_FAILURE, "The server address is required.");
    }

    if(service == NULL)
    {
        usage(binary_name, EXIT_FAILURE, "The service is required.");
    }
}


_Noreturn static void usage(const char *program_name, int exit_code, const char *message)
{
    if(message)
    {
        fprintf(stderr, "%s\n", message);
    }

    fprintf(stderr, "Usage: %s [-h] [-p <port>] <server address> <service>\n", program_name);
    fputs("Options:\n", stderr);
    fputs("  -h         Display this help message\n", stderr);
    exit(exit_code);
}

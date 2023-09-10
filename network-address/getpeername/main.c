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
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <string.h>


static void parse_arguments(int argc, char *argv[], char **server_address, char **service);
static void handle_arguments(const char *binary_name, const char *server_address, const char *service);
_Noreturn static void usage(const char *program_name, int exit_code, const char *message);


// TODO: this doesn't call getpeername
// TODO: also add getservbyname functions & notes


int main(int argc, char *argv[])
{
    char *server_address;
    char *service;

    server_address = NULL;
    service = NULL;
    parse_arguments(argc, argv, &server_address, &service);
    handle_arguments(argv[0], server_address, service);
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if(sockfd == -1)
    {
        perror("socket");
        return EXIT_FAILURE;
    }

    struct addrinfo hints, *result, *rp;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;        // IPv4
    hints.ai_socktype = SOCK_STREAM;  // TCP

    int status = getaddrinfo(server_address, service, &hints, &result);

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
    struct sockaddr_in *peer_addr = (struct sockaddr_in *) rp->ai_addr;
    char ipstr[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &(peer_addr->sin_addr), ipstr, INET_ADDRSTRLEN);
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

    if(optind + 1 >= argc)
    {
        usage(argv[0], EXIT_FAILURE, "Too few arguments.");
    }

    if(optind < argc - 2)
    {
        usage(argv[0], EXIT_FAILURE, "Too many arguments.");
    }

    *server_address = argv[optind];
    *service = argv[optind + 1];
}


static void handle_arguments(const char *binary_name, const char *server_address, const char *service)
{
    if(server_address == NULL)
    {
        usage(binary_name, EXIT_FAILURE, "");
    }

    if(service == NULL)
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

    fprintf(stderr, "Usage: %s [-h] [-p <port>] <server address> <service>\n", program_name);
    fputs("Options:\n", stderr);
    fputs("  -h         Display this help message\n", stderr);
    exit(exit_code);
}


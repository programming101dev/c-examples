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


static void parse_arguments(int argc, char *argv[], char **host_name, char **service);
static void handle_arguments(const char *binary_name, const char *host_name, const char *service);
_Noreturn static void usage(const char *program_name, int exit_code, const char *message);


#define UNKNOWN_OPTION_MESSAGE_LEN 24


int main(int argc, char *argv[])
{
    char               *host_name;
    char               *service;
    int                sockfd;
    struct sockaddr_in local_addr;
    socklen_t          addrlen;
    struct addrinfo    hints;
    struct addrinfo    *result;
    struct addrinfo    *rp;
    int                status;
    char               ipstr[INET6_ADDRSTRLEN];

    host_name = NULL;
    service   = NULL;
    parse_arguments(argc, argv, &host_name, &service);
    handle_arguments(argv[0], host_name, service);
    // TODO: this should be AF_INET or AF_INET6
    sockfd = socket(AF_INET, SOCK_STREAM | SOCK_CLOEXEC, 0);

    if(sockfd == -1)
    {
        perror("socket");
        return EXIT_FAILURE;
    }

    memset(&hints, 0, sizeof(hints));
    hints.ai_family   = AF_UNSPEC; // Allow both IPv4 and IPv6
    hints.ai_socktype = SOCK_STREAM;
    status = getaddrinfo(host_name, service, &hints, &result);

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

    // Get the local address and port number associated with the socket
    addrlen = sizeof(local_addr);

    if(getsockname(sockfd, (struct sockaddr *)&local_addr, &addrlen) == -1)
    {
        perror("getsockname");
        close(sockfd);
        freeaddrinfo(result);
        return EXIT_FAILURE;
    }

    memset(&local_addr, 0, sizeof(local_addr));
    memset(ipstr, 0, sizeof(ipstr));

    if(rp->ai_family == AF_INET)
    {
        inet_ntop(AF_INET, &(local_addr.sin_addr), ipstr, INET6_ADDRSTRLEN);
        printf("Local Address (IPv4): %s:%d\n", ipstr, ntohs(local_addr.sin_port));
    }
    else if(rp->ai_family == AF_INET6)
    {
        inet_ntop(AF_INET6, &(((struct sockaddr_in6 *)&local_addr)->sin6_addr), ipstr, INET6_ADDRSTRLEN);
        printf("Local Address (IPv6): %s:%d\n", ipstr, ntohs(local_addr.sin_port));
    }

    close(sockfd);
    freeaddrinfo(result);

    return EXIT_SUCCESS;
}


static void parse_arguments(int argc, char *argv[], char **host_name, char **service)
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
    if(optind + 1 >= argc)
    {
        usage(argv[0], EXIT_FAILURE, "Too few arguments.");
    }
    if(optind < argc - 2)
    {
        usage(argv[0], EXIT_FAILURE, "Too many arguments.");
    }
    *host_name = argv[optind];
    *service   = argv[optind + 1];
}


static void handle_arguments(const char *binary_name, const char *host_name, const char *service)
{
    if(host_name == NULL)
    {
        usage(binary_name, EXIT_FAILURE, "X");
    }
    if(service == NULL)
    {
        usage(binary_name, EXIT_FAILURE, "Y");
    }
}


_Noreturn static void usage(const char *program_name, int exit_code, const char *message)
{
    if(message)
    {
        fprintf(stderr, "%s\n", message);
    }
    fprintf(stderr, "Usage: %s [-h] <host name> <service>\n", program_name);
    fputs("Options:\n", stderr);
    fputs("  -h         Display this help message\n", stderr);
    exit(exit_code);
}

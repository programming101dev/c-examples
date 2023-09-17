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
#include <errno.h>
#include <getopt.h>
#include <inttypes.h>
#include <netinet/in.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>


static void parse_arguments(int argc, char *argv[], char **address, char **port);
static void handle_arguments(const char *binary_name, const char *address, const char *port_str, in_port_t *port);
static in_port_t parse_in_port_t(const char *binary_name, const char *port_str);
_Noreturn static void usage(const char *program_name, int exit_code, const char *message);
static int get_address_domain(const char *ip);
static int socket_create(int domain, int type, int protocol);
static void socket_bind(int sockfd, const char *address, int domain, in_port_t port);
static void print_socket_opt(int sockfd, int option_level, int option_name, const char *option_name_str);
static void socket_close(int sockfd);


int main(int argc, char *argv[])
{
    char *address;
    char *port_str;
    in_port_t port;
    int sockfd;
    int domain;

    address = NULL;
    port_str = NULL;
    parse_arguments(argc, argv, &address, &port_str);
    handle_arguments(argv[0], address, port_str, &port);
    domain = get_address_domain(address);
    sockfd = socket_create(domain, SOCK_STREAM, 0);
    socket_bind(sockfd, address, domain, port);
    print_socket_opt(sockfd, IPPROTO_TCP, SO_ACCEPTCONN, "SO_BROADCAST");
    print_socket_opt(sockfd, SOL_SOCKET, SO_BROADCAST, "SO_BROADCAST");
    print_socket_opt(sockfd, SOL_SOCKET, SO_DEBUG, "SO_DEBUG");
    print_socket_opt(sockfd, SOL_SOCKET, SO_DONTROUTE, "SO_DONTROUTE");
    print_socket_opt(sockfd, SOL_SOCKET, SO_ERROR, "SO_ERROR");
    print_socket_opt(sockfd, SOL_SOCKET, SO_KEEPALIVE, "SO_KEEPALIVE");
    print_socket_opt(sockfd, SOL_SOCKET, SO_LINGER, "SO_LINGER");
    print_socket_opt(sockfd, SOL_SOCKET, SO_OOBINLINE, "SO_OOBINLINE");
    print_socket_opt(sockfd, SOL_SOCKET, SO_RCVBUF, "SO_RCVBUF");
    print_socket_opt(sockfd, SOL_SOCKET, SO_RCVLOWAT, "SO_RCVLOWAT");
    print_socket_opt(sockfd, SOL_SOCKET, SO_RCVTIMEO, "SO_RCVTIMEO");
    print_socket_opt(sockfd, SOL_SOCKET, SO_REUSEADDR, "SO_REUSEADDR");
    print_socket_opt(sockfd, SOL_SOCKET, SO_SNDBUF, "SO_SNDBUF");
    print_socket_opt(sockfd, SOL_SOCKET, SO_SNDLOWAT, "SO_SNDLOWAT");
    print_socket_opt(sockfd, SOL_SOCKET, SO_SNDTIMEO, "SO_SNDTIMEO");
    print_socket_opt(sockfd, SOL_SOCKET, SO_TYPE, "SO_TYPE");
    socket_close(sockfd);

    return EXIT_SUCCESS;
}


static void parse_arguments(int argc, char *argv[], char **address, char **port)
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

    if (optind >= argc)
    {
        usage(argv[0], EXIT_FAILURE, "The group id is required");
    }

    if (optind < argc - 2)
    {
        usage(argv[0], EXIT_FAILURE, "Too many arguments.");
    }

    *address = argv[optind];
    *port = argv[optind + 1];
}


static void handle_arguments(const char *binary_name, const char *address, const char *port_str, in_port_t *port)
{
    if (address == NULL)
    {
        usage(binary_name, EXIT_FAILURE, "The ip address is required.");
    }

    if (port_str == NULL)
    {
        usage(binary_name, EXIT_FAILURE, "The port is required.");
    }

    *port = parse_in_port_t(binary_name, port_str);
}


in_port_t parse_in_port_t(const char *binary_name, const char *str)
{
    char *endptr;
    uintmax_t parsed_value;

    errno = 0;
    parsed_value = strtoumax(str, &endptr, 10);

    if (errno != 0)
    {
        perror("Error parsing in_port_t");
        exit(EXIT_FAILURE);
    }

    // Check if there are any non-numeric characters in the input string
    if (*endptr != '\0')
    {
        usage(binary_name, EXIT_FAILURE, "Invalid characters in input.");
    }

    // Check if the parsed value is within the valid range for in_port_t
    if (parsed_value > UINT16_MAX)
    {
        usage(binary_name, EXIT_FAILURE, "in_port_t value out of range.");
    }

    return (in_port_t)parsed_value;
}


_Noreturn static void usage(const char *program_name, int exit_code, const char *message)
{
    if (message)
    {
        fprintf(stderr, "%s\n", message);
    }

    fprintf(stderr, "Usage: %s [-h] <ip address> <port>\n", program_name);
    fputs("Options:\n", stderr);
    fputs("  -h  Display this help message\n", stderr);
    exit(exit_code);
}


static int get_address_domain(const char *address)
{
    int domain;

    if(strstr(address, ":"))
    {
        domain = AF_INET6;
    }
    else if (strstr(address, "."))
    {
        domain = AF_INET;
    }
    else
    {
        fprintf(stderr, "Invalid IP address \"%s\"\n", address);
        exit(EXIT_FAILURE);
    }

    return domain;
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


static void socket_bind(int sockfd, const char *address, int domain, in_port_t port)
{
    struct sockaddr_storage addr;

    memset(&addr, 0, sizeof(addr));

    if(inet_pton(domain, address, &addr) != 1)
    {
        perror("Invalid IP address");
        exit(EXIT_FAILURE);
    }

    if(domain == AF_INET)
    {
        struct sockaddr_in *ipv4_addr;

        ipv4_addr = (struct sockaddr_in *)&addr;
        ipv4_addr->sin_family = AF_INET;
        ipv4_addr->sin_port = htons(port);
    }
    else if(domain == AF_INET6)
    {
        struct sockaddr_in6 *ipv6_addr;

        ipv6_addr = (struct sockaddr_in6 *)&addr;
        ipv6_addr->sin6_family = AF_INET6;
        ipv6_addr->sin6_port = htons(port);
    }
    else
    {
        fprintf(stderr, "Invalid domain: %d\n", domain);
        exit(EXIT_FAILURE);
    }

    if(bind(sockfd, (struct sockaddr *)&addr, sizeof(addr)) == -1)
    {
        perror("Binding failed");
        exit(EXIT_FAILURE);
    }

    printf("Bound to socket: %s:%u\n", address, port);
}



static void print_socket_opt(int sockfd, int option_level, int option_name, const char *option_name_str)
{
    socklen_t optlen;
    int optval;
    int ret;

    optlen = sizeof(optval);
    ret = getsockopt(sockfd, option_level, option_name, &optval, &optlen);

    if (ret == 0)
    {
        printf("%s: %s\n", option_name_str, optval ? "Enabled" : "Disabled");
    }
    else
    {
        perror("getsockopt");
    }
}


static void socket_close(int client_fd)
{
    if (close(client_fd) == -1)
    {
        perror("Error closing socket");
        exit(EXIT_FAILURE);
    }
}

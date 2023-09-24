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
#include <inttypes.h>
#include <netinet/in.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>


static void parse_arguments(int argc, char *argv[], char **address, char **port, char **msg);
static void handle_arguments(const char *binary_name, const char *address, char *port_str, const char *message, in_port_t *port);
static in_port_t parse_in_port_t(const char *binary_name, const char *port_str);
_Noreturn static void usage(const char *program_name, int exit_code, const char *message);
static void convert_address(const char *address, struct sockaddr_storage *addr);
static int socket_create(int domain, int type, int protocol);
static void get_address_to_server(struct sockaddr_storage *addr, socklen_t addr_len, const char *address, int domain, in_port_t port);
static void socket_close(int sockfd);


#define UNKNOWN_OPTION_MESSAGE_LEN 24
#define BASE_TEN 10


int main(int argc, char *argv[])
{
    char                    *address;
    char                    *port_str;
    char                    *message;
    in_port_t               port;
    int                     sockfd;
    ssize_t                 bytes_sent;
    struct sockaddr_storage addr;
    address  = NULL;
    port_str = NULL;
    message  = NULL;
    parse_arguments(argc, argv, &address, &port_str, &message);
    handle_arguments(argv[0], address, port_str, message, &port);
    convert_address(address, &addr);
    sockfd = socket_create(addr.ss_family, SOCK_DGRAM, 0);
    get_address_to_server(&addr, sizeof(addr), address, addr.ss_family, port);
    bytes_sent = sendto(sockfd, message, strlen(message) + 1, 0, (struct sockaddr *)&addr, sizeof(addr));
    printf("Sent %zu bytes: \"%s\"\n", (size_t)bytes_sent, message);
    socket_close(sockfd);
    return EXIT_SUCCESS;
}


static void parse_arguments(int argc, char *argv[], char **address, char **port, char **msg)
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
    if(optind < argc - 3)
    {
        usage(argv[0], EXIT_FAILURE, "Too many arguments.");
    }
    *address = argv[optind];
    *port    = argv[optind + 1];
    *msg     = argv[optind + 2];
}


static void handle_arguments(const char *binary_name, const char *address, char *port_str, const char *message, in_port_t *port)
{
    if(address == NULL)
    {
        usage(binary_name, EXIT_FAILURE, "The address is required.");
    }
    if(port_str == NULL)
    {
        usage(binary_name, EXIT_FAILURE, "The port is required.");
    }
    if(message == NULL)
    {
        usage(binary_name, EXIT_FAILURE, "The message is required.");
    }
    *port = parse_in_port_t(binary_name, port_str);
}


in_port_t parse_in_port_t(const char *binary_name, const char *str)
{
    char      *endptr;
    uintmax_t parsed_value;
    errno        = 0;
    parsed_value = strtoumax(str, &endptr, BASE_TEN);
    if(errno != 0)
    {
        perror("Error parsing in_port_t");
        exit(EXIT_FAILURE);
    }

    // Check if there are any non-numeric characters in the input string
    if(*endptr != '\0')
    {
        usage(binary_name, EXIT_FAILURE, "Invalid characters in input.");
    }

    // Check if the parsed value is within the valid range for in_port_t
    if(parsed_value > UINT16_MAX)
    {
        usage(binary_name, EXIT_FAILURE, "in_port_t value out of range.");
    }
    return (in_port_t)parsed_value;
}


_Noreturn static void usage(const char *program_name, int exit_code, const char *message)
{
    if(message)
    {
        fprintf(stderr, "%s\n", message);
    }
    fprintf(stderr, "Usage: %s [-h] <address> <port> <message>\n", program_name);
    fputs("Options:\n", stderr);
    fputs("  -h  Display this help message\n", stderr);
    exit(exit_code);
}


static void convert_address(const char *address, struct sockaddr_storage *addr)
{
    memset(addr, 0, sizeof(*addr));
    if(inet_pton(AF_INET, address, &(((struct sockaddr_in *)addr)->sin_addr)) == 1)
    {
        // IPv4 address
        addr->ss_family = AF_INET;
    }
    else if(inet_pton(AF_INET6, address, &(((struct sockaddr_in6 *)addr)->sin6_addr)) == 1)
    {
        // IPv6 address
        addr->ss_family = AF_INET6;
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


static void get_address_to_server(struct sockaddr_storage *addr, socklen_t addr_len, const char *address, int domain, in_port_t port)
{
    memset(addr, 0, addr_len);
    if(inet_pton(domain, address, addr) != 1)
    {
        perror("Invalid IP address");
        exit(EXIT_FAILURE);
    }
    if(domain == AF_INET6)
    {
        struct sockaddr_in6 *ipv6_addr;
        ipv6_addr = (struct sockaddr_in6 *)addr;
        ipv6_addr->sin6_family = AF_INET6;
        ipv6_addr->sin6_port   = htons(port);
    }
    else if(domain == AF_INET)
    {
        struct sockaddr_in *ipv4_addr;
        ipv4_addr = (struct sockaddr_in *)addr;
        ipv4_addr->sin_family = AF_INET;
        ipv4_addr->sin_port   = htons(port);
    }
}


static void socket_close(int sockfd)
{
    if(close(sockfd) == -1)
    {
        perror("Error closing socket");
        exit(EXIT_FAILURE);
    }
}

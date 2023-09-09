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


#include <errno.h>
#include <getopt.h>
#include <netinet/in.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>


static void parse_arguments(int argc, char *argv[], char **port);
static void handle_arguments(const char *binary_name, const char *port_str, in_port_t *port);
static in_port_t parse_port(const char *binary_name, const char *port_str);
_Noreturn static void usage(const char *program_name, int exit_code, const char *message);
static void print_socket_opt(int sockfd, int option_level, int option_name, const char *option_name_str);


int main(int argc, char *argv[])
{
    char *port_str;
    in_port_t port;

    port_str = NULL;
    parse_arguments(argc, argv, &port_str);
    handle_arguments(argv[0], port_str, &port);
    printf("Port: %d\n", port);
    int listen_sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if(listen_sockfd == -1)
    {
        perror("socket");
        return EXIT_FAILURE;
    }

    // TODO: do I need to bind and listen for this to work?

    // Bind the socket to the specified port
    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(port);

    if(bind(listen_sockfd, (struct sockaddr *) &server_addr, sizeof(server_addr)) == -1)
    {
        perror("bind");
        close(listen_sockfd);
        return EXIT_FAILURE;
    }

    // Start listening for incoming connections
    if(listen(listen_sockfd, 1) == -1)
    {
        perror("listen");
        close(listen_sockfd);
        return EXIT_FAILURE;
    }

    // Print other socket options for demonstration purposes
    print_socket_opt(listen_sockfd, IPPROTO_TCP, SO_ACCEPTCONN, "SO_BROADCAST");
    print_socket_opt(listen_sockfd, SOL_SOCKET, SO_BROADCAST, "SO_BROADCAST");
    print_socket_opt(listen_sockfd, SOL_SOCKET, SO_DEBUG, "SO_DEBUG");
    print_socket_opt(listen_sockfd, SOL_SOCKET, SO_DONTROUTE, "SO_DONTROUTE");
    print_socket_opt(listen_sockfd, SOL_SOCKET, SO_ERROR, "SO_ERROR");
    print_socket_opt(listen_sockfd, SOL_SOCKET, SO_KEEPALIVE, "SO_KEEPALIVE");
    print_socket_opt(listen_sockfd, SOL_SOCKET, SO_LINGER, "SO_LINGER");
    print_socket_opt(listen_sockfd, SOL_SOCKET, SO_OOBINLINE, "SO_OOBINLINE");
    print_socket_opt(listen_sockfd, SOL_SOCKET, SO_RCVBUF, "SO_RCVBUF");
    print_socket_opt(listen_sockfd, SOL_SOCKET, SO_RCVLOWAT, "SO_RCVLOWAT");
    print_socket_opt(listen_sockfd, SOL_SOCKET, SO_RCVTIMEO, "SO_RCVTIMEO");
    print_socket_opt(listen_sockfd, SOL_SOCKET, SO_REUSEADDR, "SO_REUSEADDR");
    print_socket_opt(listen_sockfd, SOL_SOCKET, SO_SNDBUF, "SO_SNDBUF");
    print_socket_opt(listen_sockfd, SOL_SOCKET, SO_SNDLOWAT, "SO_SNDLOWAT");
    print_socket_opt(listen_sockfd, SOL_SOCKET, SO_SNDTIMEO, "SO_SNDTIMEO");
    print_socket_opt(listen_sockfd, SOL_SOCKET, SO_TYPE, "SO_TYPE");

    // Close the listening socket
    close(listen_sockfd);

    return EXIT_SUCCESS;
}


static void parse_arguments(int argc, char *argv[], char **port)
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
        usage(argv[0], EXIT_FAILURE, "The port is required");
    }
    else if(optind < argc - 1)
    {
        usage(argv[0], EXIT_FAILURE, "Too many arguments.");
    }

    *port = argv[optind];
}


static void handle_arguments(const char *binary_name, const char *port_str, in_port_t *port)
{
    if(port_str == NULL)
    {
        usage(binary_name, EXIT_FAILURE, "");
    }

    *port = parse_port(binary_name, port_str);
}


static in_port_t parse_port(const char *binary_name, const char *port_str)
{
    char *endptr;
    long int parsed_port;

    errno = 0;
    parsed_port = strtol(port_str, &endptr, 10);

    if (errno != 0)
    {
        usage(binary_name, EXIT_FAILURE, "Error parsing port number.");
    }

    // Check if there are any non-numeric characters in port_str
    if(*endptr != '\0')
    {
        usage(binary_name, EXIT_FAILURE, "Invalid characters in port number.");
    }

    // Check if the port is within the valid range
    if(parsed_port < 0 || parsed_port > UINT16_MAX)
    {
        usage(binary_name, EXIT_FAILURE, "Port number out of range.");
    }

    return (in_port_t)parsed_port;
}


_Noreturn static void usage(const char *program_name, int exit_code, const char *message)
{
    if(message)
    {
        fprintf(stderr, "%s\n", message);
    }

    fprintf(stderr, "Usage: %s [-h] <port>\n", program_name);
    fputs("Options:\n", stderr);
    fputs("  -h  Display this help message\n", stderr);
    exit(exit_code);
}


static void print_socket_opt(int sockfd, int option_level, int option_name, const char *option_name_str)
{
    socklen_t optlen;
    int optval;
    int ret;

    optlen = sizeof(optval);
    ret = getsockopt(sockfd, option_level, option_name, &optval, &optlen);

    if(ret == 0)
    {
        printf("%s: %s\n", option_name_str, optval ? "Enabled" : "Disabled");
    }
    else
    {
        perror("getsockopt");
    }
}

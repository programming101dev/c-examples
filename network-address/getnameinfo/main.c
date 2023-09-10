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
#include <netdb.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>


static void parse_arguments(int argc, char *argv[], char **server_address, char **port);
static void handle_arguments(const char *binary_name, const char *server_address, const char *port_str, in_port_t *port);
static in_port_t parse_port(const char *binary_name, const char *port_str);
_Noreturn static void usage(const char *program_name, int exit_code, const char *message);
static void display_address(struct sockaddr_in addr, socklen_t addrlen);


int main(int argc, char *argv[])
{
    char *ip_address;
    char *port_str;
    in_port_t port;
    struct sockaddr_in addr;
    socklen_t addrlen = sizeof(addr);

    parse_arguments(argc, argv, &ip_address, &port_str);
    handle_arguments(argv[0], ip_address, port_str, &port);
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;

    if(inet_pton(AF_INET, ip_address, &(addr.sin_addr)) <= 0)
    {
        perror("inet_pton");
        return EXIT_FAILURE;
    }

    addr.sin_port = htons(port);
    display_address(addr, addrlen);

    return EXIT_SUCCESS;
}


static void parse_arguments(int argc, char *argv[], char **server_address, char **port)
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
    *port = argv[optind + 1];
}


static void handle_arguments(const char *binary_name, const char *ip_address, const char *port_str, in_port_t *port)
{
    if(ip_address == NULL)
    {
        usage(binary_name, EXIT_FAILURE, "");
    }

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

    fprintf(stderr, "Usage: %s [-h] [-p <port>] <server address>\n", program_name);
    fputs("Options:\n", stderr);
    fputs("  -h         Display this help message\n", stderr);
    fputs("  -p <port>  THe port to connect to\n", stderr);
    exit(exit_code);
}


static void display_address(struct sockaddr_in addr, socklen_t addrlen)
{
    char hostname[NI_MAXHOST];
    char port[NI_MAXSERV];

    // Get the hostname and port number associated with the address
    int result = getnameinfo((struct sockaddr *) &addr, addrlen, hostname, NI_MAXHOST, port, NI_MAXSERV, NI_NUMERICSERV);

    if(result == 0)
    {
        printf("Address: %s:%s\n", hostname, port);
    }
    else
    {
        fprintf(stderr, "getnameinfo error: %s\n", gai_strerror(result));
        exit(EXIT_FAILURE);
    }
}
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
#include <inttypes.h>
#include <netdb.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>


static void parse_arguments(int argc, char *argv[], char **server_address, char **port);
static void handle_arguments(const char *binary_name, const char *server_address, const char *port_str, in_port_t *port);
static in_port_t parse_in_port_t(const char *binary_name, const char *port_str);
_Noreturn static void usage(const char *program_name, int exit_code, const char *message);
static void display_address(struct sockaddr_storage *addr, socklen_t addrlen);


#define UNKNOWN_OPTION_MESSAGE_LEN 24
#define BASE_TEN 10


int main(int argc, char *argv[])
{
    char                    *server_address;
    char                    *port_str;
    in_port_t               port;
    struct sockaddr_storage addr;
    socklen_t               addrlen;
    struct addrinfo         hints;
    struct addrinfo         *result;
    int                     error;

    parse_arguments(argc, argv, &server_address, &port_str);
    handle_arguments(argv[0], server_address, port_str, &port);
    memset(&hints, 0, sizeof(hints));
    hints.ai_family   = AF_UNSPEC; // Allow both IPv4 and IPv6
    hints.ai_socktype = SOCK_STREAM;
    error = getaddrinfo(server_address, port_str, &hints, &result);

    if(error != 0)
    {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(error));
        return EXIT_FAILURE;
    }

    if(result == NULL)
    {
        fprintf(stderr, "getaddrinfo returned no address information\n");
        return EXIT_FAILURE;
    }

    if(result->ai_addrlen > sizeof(addr))
    {
        fprintf(stderr, "Address size exceeds sockaddr_storage capacity\n");
        return EXIT_FAILURE;
    }

    memcpy(&addr, result->ai_addr, result->ai_addrlen);
    addrlen = result->ai_addrlen;
    display_address(&addr, addrlen);
    freeaddrinfo(result);

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
    *port           = argv[optind + 1];
}


static void handle_arguments(const char *binary_name, const char *server_address, const char *port_str, in_port_t *port)
{
    if(server_address == NULL)
    {
        usage(binary_name, EXIT_FAILURE, "The ip address is required.");
    }

    if(port_str == NULL)
    {
        usage(binary_name, EXIT_FAILURE, "The port is required.");
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

    fprintf(stderr, "Usage: %s [-h] <server address> <port>\n", program_name);
    fputs("Options:\n", stderr);
    fputs("  -h         Display this help message\n", stderr);
    fputs("  -p <port>  THe port to connect to\n", stderr);
    exit(exit_code);
}


static void display_address(struct sockaddr_storage *addr, socklen_t addrlen)
{
    if(addr->ss_family == AF_INET || addr->ss_family == AF_INET6)
    {
        char hostname[NI_MAXHOST];
        char port[NI_MAXSERV];
        int  result;

        result = getnameinfo((struct sockaddr *)addr, addrlen, hostname, NI_MAXHOST, port, NI_MAXSERV, NI_NUMERICSERV);

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
    else
    {
        fprintf(stderr, "Unsupported address family: %d\n", addr->ss_family);
        exit(EXIT_FAILURE);
    }
}

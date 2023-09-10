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
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>


static void parse_arguments(int argc, char *argv[], char **ip_address, char **port);
static void handle_arguments(const char *binary_name, const char *ip_address, const char *port_str, in_port_t *port);
static in_port_t parse_port(const char *binary_name, const char *port_str);
_Noreturn static void usage(const char *program_name, int exit_code, const char *message);
static int create_socket(void);
static struct sockaddr_in prepare_server_address(int client_fd, const char *ip_address, in_port_t port);
static void connect_to_server(int client_fd, struct sockaddr_in server_addr);
static void do_communication(int client_fd);
static void close_socket(int client_fd);


int main(int argc, char *argv[])
{
    char *ip_address;
    char *port_str;
    in_port_t port;

    ip_address = NULL;
    port_str = NULL;
    parse_arguments(argc, argv, &ip_address, &port_str);
    handle_arguments(argv[0], ip_address, port_str, &port);
    int client_fd = create_socket();
    struct sockaddr_in server_addr = prepare_server_address(client_fd, ip_address, port);
    connect_to_server(client_fd, server_addr);
    printf("Connected to the server\n");
    do_communication(client_fd);
    close_socket(client_fd);

    return EXIT_SUCCESS;
}


static void parse_arguments(int argc, char *argv[], char **ip_address, char **port)
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

    *ip_address = argv[optind];
    *port = argv[optind + 1];
}


static void handle_arguments(const char *binary_name, const char *ip_address,const char *port_str, in_port_t *port)
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

    fprintf(stderr, "Usage: %s [-h] <ip address> <port>\n", program_name);
    fputs("Options:\n", stderr);
    fputs("  -h  Display this help message\n", stderr);
    exit(exit_code);
}


static int create_socket(void)
{
    int client_fd = socket(AF_INET, SOCK_STREAM, 0);

    if(client_fd == -1)
    {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    return client_fd;
}


static struct sockaddr_in prepare_server_address(int client_fd, const char *ip_address, in_port_t port)
{
    struct sockaddr_in server_addr;

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);

    if(inet_pton(AF_INET, ip_address, &server_addr.sin_addr) <= 0)
    {
        perror("Invalid address/Address not supported");
        close_socket(client_fd);
        exit(EXIT_FAILURE);
    }

    return server_addr;
}


static void connect_to_server(int client_fd, struct sockaddr_in server_addr)
{
    if(connect(client_fd, (struct sockaddr *) &server_addr, sizeof(server_addr)) == -1)
    {
        perror("Connection failed");
        close_socket(client_fd);
        exit(EXIT_FAILURE);
    }
}


#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"

static void do_communication(int client_fd)
{
    // Perform communication or other actions here
}

#pragma GCC diagnostic pop


static void close_socket(int client_fd)
{
    if(close(client_fd) == -1)
    {
        perror("Error closing socket");
        exit(EXIT_FAILURE);
    }
}

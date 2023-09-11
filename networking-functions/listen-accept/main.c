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
#include <limits.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>


static void parse_arguments(int argc, char *argv[], char **ip_address, char **port, char **backlog);
static void handle_arguments(const char *binary_name, const char *ip_address, const char *port_str, const char *backlog_str, in_port_t *port, int *backlog);
static in_port_t parse_in_port_t(const char *binary_name, const char *port_str);
static int parse_positive_int(const char *binary_name, const char *str);
_Noreturn static void usage(const char *program_name, int exit_code, const char *message);
static int create_socket(void);
static void bind_socket(int server_fd, const char *ip_address, in_port_t port);
static void start_listening(int server_fd, int backlog);
static int accept_connection(int server_fd);


int main(int argc, char *argv[])
{
    char *ip_address;
    char *port_str;
    char *backlog_str;
    in_port_t port;
    int backlog;
    int server_fd;
    int client_fd;

    ip_address = NULL;
    port_str = NULL;
    backlog_str = NULL;
    parse_arguments(argc, argv, &ip_address, &port_str, &backlog_str);
    handle_arguments(argv[0], ip_address, port_str, backlog_str, &port, &backlog);
    server_fd = create_socket();
    bind_socket(server_fd, ip_address, port);
    start_listening(server_fd, backlog);
    client_fd = accept_connection(server_fd);

    if(close(client_fd) < 0)
    {
        perror("Failed to close client socket");
        return EXIT_FAILURE;
    }

    if(close(server_fd) < 0)
    {
        perror("Failed to close server socket");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}


static void parse_arguments(int argc, char *argv[], char **ip_address, char **port, char **backlog)
{
    int opt;

    opterr = 0;

    while((opt = getopt(argc, argv, "hp:b:")) != -1)
    {
        switch(opt)
        {
            case 'b':
            {
                *backlog = optarg;
                break;
            }
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
        usage(argv[0], EXIT_FAILURE, "The group id is required");
    }

    if(optind < argc - 2)
    {
        usage(argv[0], EXIT_FAILURE, "Too many arguments.");
    }

    *ip_address = argv[optind];
    *port = argv[optind + 1];
}


static void handle_arguments(const char *binary_name, const char *ip_address, const char *port_str, const char *backlog_str, in_port_t *port, int *backlog)
{
    if(ip_address == NULL)
    {
        usage(binary_name, EXIT_FAILURE, "The ip address is required.");
    }

    if(port_str == NULL)
    {
        usage(binary_name, EXIT_FAILURE, "The port is required.");
    }

    if(backlog_str == NULL)
    {
        usage(binary_name, EXIT_FAILURE, "The backlog is required.");
    }

    *port = parse_in_port_t(binary_name, port_str);
    *backlog = parse_positive_int(binary_name, backlog_str);
}


in_port_t parse_in_port_t(const char *binary_name, const char *str)
{
    char *endptr;
    uintmax_t parsed_value;

    errno = 0;
    parsed_value = strtoumax(str, &endptr, 10);

    if(errno != 0)
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

int parse_positive_int(const char *binary_name, const char *str)
{
    char *endptr;
    intmax_t parsed_value;

    errno = 0;
    parsed_value = strtoimax(str, &endptr, 10);

    if (errno != 0)
    {
        usage(binary_name, EXIT_FAILURE, "Error parsing integer.");
    }

    // Check if there are any non-numeric characters in the input string
    if (*endptr != '\0')
    {
        usage(binary_name, EXIT_FAILURE, "Invalid characters in input.");
    }

    // Check if the parsed value is non-negative
    if (parsed_value < 0 || parsed_value > INT_MAX)
    {
        usage(binary_name, EXIT_FAILURE, "Integer out of range or negative.");
    }

    return (int)parsed_value;
}


_Noreturn static void usage(const char *program_name, int exit_code, const char *message)
{
    if(message)
    {
        fprintf(stderr, "%s\n", message);
    }

    fprintf(stderr, "Usage: %s [-h] -b <backlog> <ip address> <port>\n", program_name);
    fputs("Options:\n", stderr);
    fputs("  -h  Display this help message\n", stderr);
    fputs("  -b <backlog> the backlog\n", stderr);
    exit(exit_code);
}


static int create_socket(void)
{
    int server_fd;

    server_fd = socket(AF_INET, SOCK_STREAM, 0);

    if(server_fd == -1)
    {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }

    return server_fd;
}


static void bind_socket(int server_fd, const char *ip_address, in_port_t port)
{
    struct sockaddr_in server_addr;

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    inet_pton(AF_INET, ip_address, &(server_addr.sin_addr));
    server_addr.sin_port = htons(port);

    if(bind(server_fd, (struct sockaddr *) &server_addr, sizeof(server_addr)) == -1)
    {
        perror("bind failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    printf("Socket bound to %s:%d\n", ip_address, port);
}


static void start_listening(int server_fd, int backlog)
{
    if(listen(server_fd, backlog) == -1)
    {
        perror("listen failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    printf("Listening for incoming connections...\n");
}


static int accept_connection(int server_fd)
{
    struct sockaddr_in client_addr;
    socklen_t client_len = sizeof(client_addr);

    int client_fd = accept(server_fd, (struct sockaddr *) &client_addr, &client_len);
    if(client_fd == -1)
    {
        perror("accept failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    printf("Accepted a new connection\n");

    return client_fd;
}

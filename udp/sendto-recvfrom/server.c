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


static void parse_arguments(int argc, char *argv[], char **ip_address, char **port, char **msg);
static void handle_arguments(const char *binary_name, const char *ip_address, char *port_str, const char *message, in_port_t *port);
static in_port_t parse_in_port_t(const char *binary_name, const char *port_str);
_Noreturn static void usage(const char *program_name, int exit_code, const char *message);


int main(int argc, char *argv[])
{
    char *ip_address;
    char *port_str;
    char *message;
    in_port_t port;
    int sockfd;
    struct sockaddr_in server_addr, client_addr;
    char buffer[1024];
    socklen_t client_addr_len;
    ssize_t bytes_received;
    ssize_t bytes_sent;

    ip_address = NULL;
    port_str = NULL;
    parse_arguments(argc, argv, &ip_address, &port_str, &message);
    handle_arguments(argv[0], ip_address, port_str, message, &port);
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    if(sockfd == -1)
    {
        perror("socket");
        return EXIT_FAILURE;
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    inet_pton(AF_INET, ip_address, &(server_addr.sin_addr));

    if(bind(sockfd, (struct sockaddr *) &server_addr, sizeof(server_addr)) == -1)
    {
        perror("bind");
        close(sockfd);
        return EXIT_FAILURE;
    }

    printf("Server listening on port %d...\n", port);

    client_addr_len = sizeof(client_addr);
    bytes_received = recvfrom(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr *) &client_addr, &client_addr_len);

    if(bytes_received == -1)
    {
        perror("recvfrom");
        close(sockfd);
        return EXIT_FAILURE;
    }

    buffer[bytes_received] = '\0';
    printf("Received from client: %s\n", buffer);

    bytes_sent = sendto(sockfd, message, strlen(message), 0, (struct sockaddr *) &client_addr, client_addr_len);
    if(bytes_sent == -1)
    {
        perror("sendto");
        close(sockfd);
        return EXIT_FAILURE;
    }

    close(sockfd);

    return EXIT_SUCCESS;
}


static void parse_arguments(int argc, char *argv[], char **ip_address, char **port, char **msg)
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
        usage(argv[0], EXIT_FAILURE, "The group id is required");
    }

    if(optind < argc - 3)
    {
        usage(argv[0], EXIT_FAILURE, "Too many arguments.");
    }

    *ip_address = argv[optind];
    *port = argv[optind + 1];
    *msg = argv[optind + 2];
}


static void handle_arguments(const char *binary_name, const char *ip_address, char *port_str, const char *message, in_port_t *port)
{
    if(ip_address == NULL)
    {
        usage(binary_name, EXIT_FAILURE, "The ip address is required.");
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

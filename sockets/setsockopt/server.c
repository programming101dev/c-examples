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
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>


// TODO: don't use ANY for the ip address?


static void parse_arguments(int argc, char *argv[], char **port);
static void handle_arguments(const char *binary_name, const char *port_str, in_port_t *port);
static in_port_t parse_port(const char *binary_name, const char *port_str);
_Noreturn static void usage(const char *program_name, int exit_code, const char *message);


int main(int argc, char *argv[])
{
    char *port_str;
    in_port_t port;

    port_str = NULL;
    parse_arguments(argc, argv, &port_str);
    handle_arguments(argv[0], port_str, &port);
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if(sockfd == -1)
    {
        perror("socket");
        return EXIT_FAILURE;
    }

    int optval = 1;
    if(setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) == -1)
    {
        perror("setsockopt");
        close(sockfd);
        return EXIT_FAILURE;
    }

    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(port);

    // Bind the server socket to the specified address and port
    if(bind(sockfd, (struct sockaddr *) &server_addr, sizeof(server_addr)) == -1)
    {
        perror("bind");
        close(sockfd);
        return EXIT_FAILURE;
    }

    // Start listening for incoming connections
    if(listen(sockfd, 1) == -1)
    {
        perror("listen");
        close(sockfd);
        return EXIT_FAILURE;
    }

    printf("Server is listening on port 8888...\n");

    // Accept incoming connections
    struct sockaddr_in client_addr;
    socklen_t client_addr_len = sizeof(client_addr);
    int client_sockfd = accept(sockfd, (struct sockaddr *) &client_addr, &client_addr_len);
    if(client_sockfd == -1)
    {
        perror("accept");
        close(sockfd);
        return EXIT_FAILURE;
    }

    printf("Client connected!\n");

    close(client_sockfd);
    close(sockfd);

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
        usage(argv[0], EXIT_FAILURE, "The group id is required");
    }

    if(optind < argc - 1)
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

    fprintf(stderr, "Usage: %s [-h] <file path>\n", program_name);
    fputs("Options:\n", stderr);
    fputs("  -h  Display this help message\n", stderr);
    exit(exit_code);
}

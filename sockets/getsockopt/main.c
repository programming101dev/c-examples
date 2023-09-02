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


#include <getopt.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>


static void usage(const char *program_name, int exit_code, const char *message);
static void print_socket_opt(int sockfd, int option_level, int option_name, const char *option_name_str);


int main(int argc, char *argv[])
{
    int port = 0;
    int opt;

    while((opt = getopt(argc, argv, "hp:")) != -1)
    {
        switch(opt)
        {
            case 'p':
                // Convert the port argument to an integer using strtol
                port = (int) strtol(optarg, NULL, 10);

                if(port == 0)
                {
                    fprintf(stderr, "Invalid port number\n");
                    return 1;
                }
                break;
            case 'h':
                usage(argv[0], EXIT_SUCCESS, NULL);
                break;
            default:
                usage(argv[0], EXIT_FAILURE, NULL);
        }
    }

    if(port == 0)
    {
        fprintf(stderr, "Port number not specified. Use -p <port> to set the port.\n");
        return EXIT_FAILURE;
    }

    printf("Port: %d\n", port);

    // Create a listening socket
    int listen_sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if(listen_sockfd == -1)
    {
        perror("socket");
        return EXIT_FAILURE;
    }

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


static void usage(const char *program_name, int exit_code, const char *message)
{
    if(message)
    {
        fputs(message, stderr);
    }

    fprintf(stderr, "Usage: %s -p <port>\n", program_name);
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

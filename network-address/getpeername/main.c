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


#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <string.h>


static void display_help(const char *program_name);


int main(int argc, char *argv[])
{
    int opt;
    char *server_address = NULL;
    const char *port = NULL;

    while((opt = getopt(argc, argv, "hp:")) != -1)
    {
        switch(opt)
        {
            case 'h':
            {
                display_help(argv[0]);
                return EXIT_SUCCESS;
            }
            case 'p':
            {
                port = optarg;
                break;
            }
            default:
            {
                display_help(argv[0]);
                return EXIT_FAILURE;
            }
        }
    }

    if(optind >= argc)
    {
        display_help(argv[0]);
        return EXIT_FAILURE;
    }

    server_address = argv[optind];

    if(port == NULL)
    {
        printf("Port not provided. Using default port 8080.\n");
        port = "8080";
    }

    int sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if(sockfd == -1)
    {
        perror("socket");
        return EXIT_FAILURE;
    }

    struct addrinfo hints, *result, *rp;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;        // IPv4
    hints.ai_socktype = SOCK_STREAM;  // TCP

    int status = getaddrinfo(server_address, port, &hints, &result);

    if(status != 0)
    {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
        close(sockfd);
        return EXIT_FAILURE;
    }

    // Attempt to connect to the server using the available address info
    for(rp = result; rp != NULL; rp = rp->ai_next)
    {
        if(connect(sockfd, rp->ai_addr, rp->ai_addrlen) == 0)
        {
            break; // Connected successfully
        }
    }

    if(rp == NULL)
    {
        perror("connect");
        close(sockfd);
        freeaddrinfo(result);
        return EXIT_FAILURE;
    }

    // Get the peer address and port number associated with the socket
    struct sockaddr_in *peer_addr = (struct sockaddr_in *) rp->ai_addr;
    char ipstr[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &(peer_addr->sin_addr), ipstr, INET_ADDRSTRLEN);
    printf("Connected to: %s:%s\n", ipstr, port);

    freeaddrinfo(result);
    close(sockfd);

    return EXIT_SUCCESS;
}


static void display_help(const char *program_name)
{
    printf("Usage: %s -p <port> <server_address>\n", program_name);
}


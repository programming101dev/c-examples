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
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>
#include <netdb.h>


static void usage(const char *program_name, int exit_code, const char *message);


int main(int argc, char *argv[])
{
    int opt;
    char *port = NULL;
    char *hostname = NULL;

    while((opt = getopt(argc, argv, "hp:")) != -1)
    {
        switch(opt)
        {
            case 'p':
                port = optarg;
                break;
            case 'h':
                usage(argv[0], EXIT_SUCCESS, NULL);
                break;
            default:
                usage(argv[0], EXIT_FAILURE, NULL);
        }
    }

    if(port == NULL || optind >= argc)
    {
        usage(argv[0], EXIT_FAILURE, "");
    }

    hostname = argv[optind];

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

    int status = getaddrinfo(hostname, port, &hints, &result);
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

    // Get the local address and port number associated with the socket
    struct sockaddr_in local_addr;
    socklen_t addrlen = sizeof(local_addr);
    if(getsockname(sockfd, (struct sockaddr *) &local_addr, &addrlen) == -1)
    {
        perror("getsockname");
        close(sockfd);
        freeaddrinfo(result);
        return EXIT_FAILURE;
    }

    char ipstr[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &(local_addr.sin_addr), ipstr, INET_ADDRSTRLEN);
    printf("Local Address: %s:%d\n", ipstr, ntohs(local_addr.sin_port));

    freeaddrinfo(result);
    close(sockfd);
    return EXIT_SUCCESS;
}


static void usage(const char *program_name, int exit_code, const char *message)
{
    if(message)
    {
        fputs(message, stderr);
    }

    fprintf(stderr, "Usage: %s -p <port> <hostname>\n", program_name);
    exit(exit_code);
}

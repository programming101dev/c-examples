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
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>


static void usage(const char *program_name, int exit_code, const char *message);
static int resolve_hostname_to_ip(const char *hostname);


int main(int argc, char *argv[])
{
    int option;

    while((option = getopt(argc, argv, "h")) != -1)
    {
        switch(option)
        {
            case 'h':
            {
                usage(argv[0], EXIT_SUCCESS, NULL);
                break;
            }
            case '?':
            {
                char message[24];

                snprintf(message, sizeof(message), "Unknown option '-%c'.\n", optopt);
                usage(argv[0], EXIT_FAILURE, message);
                break;
            }
            default:
            {
                usage(argv[0], EXIT_FAILURE, NULL);
            }
        }
    }

    if(argc != optind + 1)
    {
        usage(argv[0], EXIT_FAILURE, "");
    }

    char *hostname = argv[optind];
    int result = resolve_hostname_to_ip(hostname);
    return result;
}


static void usage(const char *program_name, int exit_code, const char *message)
{
    if(message)
    {
        fputs(message, stderr);
    }

    fprintf(stderr, "Usage: %s [-h] hostname\n", program_name);
    exit(exit_code);
}


static int resolve_hostname_to_ip(const char *hostname)
{
    struct addrinfo hints, *result, *res;
    int error;
    char ipstr[INET6_ADDRSTRLEN];

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC; // Allow both IPv4 and IPv6
    hints.ai_socktype = SOCK_STREAM; // Use TCP socket type

    error = getaddrinfo(hostname, NULL, &hints, &result);
    if(error != 0)
    {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(error));
        return EXIT_FAILURE;
    }

    // Print all IP addresses associated with the hostname
    for(res = result; res != NULL; res = res->ai_next)
    {
        void *addr;
        const char *ipver;

        if(res->ai_family == AF_INET)
        {
            // IPv4
            struct sockaddr_in *ipv4 = (struct sockaddr_in *) res->ai_addr;
            addr = &(ipv4->sin_addr);
            ipver = "IPv4";
        }
        else
        {
            // IPv6
            struct sockaddr_in6 *ipv6 = (struct sockaddr_in6 *) res->ai_addr;
            addr = &(ipv6->sin6_addr);
            ipver = "IPv6";
        }

        inet_ntop(res->ai_family, addr, ipstr, sizeof(ipstr));
        printf("Hostname: %s, IP Address (%s): %s\n", hostname, ipver, ipstr);
    }

    freeaddrinfo(result);
    return EXIT_SUCCESS;
}

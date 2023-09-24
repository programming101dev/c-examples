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
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>


static void parse_arguments(int argc, char *argv[], char **host_name);
static void handle_arguments(const char *binary_name, const char *host_name);
_Noreturn static void usage(const char *program_name, int exit_code, const char *message);
static int resolve_hostname_to_ip(const char *hostname);


#define UNKNOWN_OPTION_MESSAGE_LEN 24


int main(int argc, char *argv[])
{
    char *host_name;
    int  result;
    host_name = NULL;
    parse_arguments(argc, argv, &host_name);
    handle_arguments(argv[0], host_name);
    result = resolve_hostname_to_ip(host_name);
    return result;
}


static void parse_arguments(int argc, char *argv[], char **host_name)
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
    if(optind >= argc)
    {
        usage(argv[0], EXIT_FAILURE, "The host name is required");
    }
    else if(optind < argc - 1)
    {
        usage(argv[0], EXIT_FAILURE, "Too many arguments.");
    }
    *host_name = argv[optind];
}


static void handle_arguments(const char *binary_name, const char *host_name)
{
    if(host_name == NULL)
    {
        usage(binary_name, EXIT_FAILURE, "The host name is required.");
    }
}


_Noreturn static void usage(const char *program_name, int exit_code, const char *message)
{
    if(message)
    {
        fprintf(stderr, "%s\n", message);
    }
    fprintf(stderr, "Usage: %s [-h] <host name>\n", program_name);
    fputs("Options:\n", stderr);
    fputs("  -h  Display this help message\n", stderr);
    exit(exit_code);
}


static int resolve_hostname_to_ip(const char *hostname)
{
    struct addrinfo hints;
    struct addrinfo *result;
    struct addrinfo *res;
    int             error;
    char            ipstr[INET6_ADDRSTRLEN];
    memset(&hints, 0, sizeof(hints));
    hints.ai_family   = AF_UNSPEC; // Allow both IPv4 and IPv6
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
        struct sockaddr_in      *ipv4;
        struct sockaddr_in6     *ipv6;
        void                    *addr;
        const char              *ipver;
        struct sockaddr_storage temp;
        if(res->ai_family == AF_INET)
        {
            memcpy(&temp, res->ai_addr, sizeof(struct sockaddr_in));
            ipv4  = (struct sockaddr_in *)&temp;
            addr  = &(ipv4->sin_addr);
            ipver = "IPv4";
        }
        else if(res->ai_family == AF_INET6)
        {
            memcpy(&temp, res->ai_addr, sizeof(struct sockaddr_in6));
            ipv6  = (struct sockaddr_in6 *)&temp;
            addr  = &(ipv6->sin6_addr);
            ipver = "IPv6";
        }
        else
        {
            addr  = NULL;
            ipver = NULL;
        }
        if(addr)
        {
            inet_ntop(res->ai_family, addr, ipstr, sizeof(ipstr));
            printf("Hostname: %s, IP Address (%s): %s\n", hostname, ipver, ipstr);
        }
        else
        {
            fprintf(stderr, "Unsupported address family: %d\n", res->ai_family);
        }
    }
    freeaddrinfo(result);
    return EXIT_SUCCESS;
}

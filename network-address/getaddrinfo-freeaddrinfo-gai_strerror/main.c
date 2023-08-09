#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>


static void print_help(const char *program_name);
static int resolve_hostname_to_ip(const char *hostname);


int main(int argc, char* argv[])
{
    int option;
    while((option = getopt(argc, argv, "h")) != -1) {
        switch (option) {
            case 'h':
                print_help(argv[0]);
                return EXIT_SUCCESS;
            default:
                print_help(argv[0]);
                return EXIT_FAILURE;
        }
    }

    if(argc != optind + 1) {
        print_help(argv[0]);
        return EXIT_FAILURE;
    }

    char* hostname = argv[optind];
    int result = resolve_hostname_to_ip(hostname);
    return result;
}


static void print_help(const char *program_name)
{
    printf("Usage: %s [-h] hostname\n", program_name);
    printf("Resolve the IP addresses associated with the given hostname.\n");
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
    if(error != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(error));
        return EXIT_FAILURE;
    }

    // Print all IP addresses associated with the hostname
    for(res = result; res != NULL; res = res->ai_next)
    {
        void* addr;
        const char* ipver;

        if(res->ai_family == AF_INET)
        {
            // IPv4
            struct sockaddr_in* ipv4 = (struct sockaddr_in*)res->ai_addr;
            addr = &(ipv4->sin_addr);
            ipver = "IPv4";
        }
        else
        {
            // IPv6
            struct sockaddr_in6* ipv6 = (struct sockaddr_in6*)res->ai_addr;
            addr = &(ipv6->sin6_addr);
            ipver = "IPv6";
        }

        inet_ntop(res->ai_family, addr, ipstr, sizeof(ipstr));
        printf("Hostname: %s, IP Address (%s): %s\n", hostname, ipver, ipstr);
    }

    freeaddrinfo(result);
    return EXIT_SUCCESS;
}

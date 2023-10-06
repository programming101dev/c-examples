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
#include <ifaddrs.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>

int main(int argc, char *argv[])
{
    struct ifaddrs *interfaces;
    struct ifaddrs *ifaddr;
    char            host[NI_MAXHOST];

    // Get the list of network interfaces
    if(getifaddrs(&interfaces) == -1)
    {
        perror("getifaddrs");
        return 1;
    }

    printf("List of Network Interfaces:\n");

    for(ifaddr = interfaces; ifaddr != NULL; ifaddr = ifaddr->ifa_next)
    {
        if(ifaddr->ifa_addr == NULL)
        {
            continue;
        }

        if(ifaddr->ifa_addr->sa_family == AF_INET)
        {
            struct sockaddr_in ipv4;

            memcpy(&ipv4, ifaddr->ifa_addr, sizeof(struct sockaddr_in));
            inet_ntop(AF_INET, &(ipv4.sin_addr), host, NI_MAXHOST);
        }
        else if(ifaddr->ifa_addr->sa_family == AF_INET6)
        {
            struct sockaddr_in6 ipv6;

            memcpy(&ipv6, ifaddr->ifa_addr, sizeof(struct sockaddr_in6));
            inet_ntop(AF_INET6, &(ipv6.sin6_addr), host, NI_MAXHOST);
        }
        else
        {
            continue;
        }

        printf("%-12s : %s\n", ifaddr->ifa_name, host);
    }

    freeifaddrs(interfaces);

    return EXIT_SUCCESS;
}

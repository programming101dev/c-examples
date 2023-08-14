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
#include <stdlib.h>
#include <string.h>
#include <ifaddrs.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>


int main(void)
{
    struct ifaddrs *interfaces, *ifaddr;
    char host[NI_MAXHOST];

    // Get the list of network interfaces
    if(getifaddrs(&interfaces) == -1)
    {
        perror("getifaddrs");
        return 1;
    }

    printf("List of Network Interfaces:\n");

    // Iterate through the linked list of interfaces
    for(ifaddr = interfaces; ifaddr != NULL; ifaddr = ifaddr->ifa_next)
    {
        if(ifaddr->ifa_addr == NULL)
        {
            continue;
        }

        // Get the IP address as a string
        if(ifaddr->ifa_addr->sa_family == AF_INET)
        {
            struct sockaddr_in *s4 = (struct sockaddr_in *) ifaddr->ifa_addr;
            inet_ntop(AF_INET, &s4->sin_addr, host, NI_MAXHOST);
        }
        else if(ifaddr->ifa_addr->sa_family == AF_INET6)
        {
            struct sockaddr_in6 *s6 = (struct sockaddr_in6 *) ifaddr->ifa_addr;
            inet_ntop(AF_INET6, &s6->sin6_addr, host, NI_MAXHOST);
        }
        else
        {
            continue;
        }

        printf("%-12s : %s\n", ifaddr->ifa_name, host);
    }

    // Free the memory allocated by getifaddrs
    freeifaddrs(interfaces);

    return EXIT_SUCCESS;
}

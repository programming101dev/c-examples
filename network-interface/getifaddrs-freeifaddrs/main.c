/*
 * Copyright 2026 D'Arcy Smith
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <arpa/inet.h>
#include <ifaddrs.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>

int main(void)
{
    struct ifaddrs       *interfaces;
    const struct ifaddrs *ifaddr;
    char                  host[NI_MAXHOST];

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

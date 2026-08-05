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
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>

#define BITS_TO_SHIFT_8 8
#define BITS_TO_SHIFT_16 16
#define BITS_TO_SHIFT_24 24

int main(void)
{
    // TODO pass these in on the command line
    const char     *ipv4_address_str = "192.168.0.1";
    const char     *ipv6_address_str = "2001:0db8:85a3:0000:0000:8a2e:0370:7334";
    struct in_addr  ipv4_addr;
    struct in6_addr ipv6_addr;

    // Convert IPv4 address from string to binary format (network byte order)
    if(inet_pton(AF_INET, ipv4_address_str, &ipv4_addr) == 1)
    {
        printf("IPv4 Address: %u.%u.%u.%u\n", (unsigned char)ipv4_addr.s_addr, (unsigned char)(ipv4_addr.s_addr >> BITS_TO_SHIFT_8), (unsigned char)(ipv4_addr.s_addr >> BITS_TO_SHIFT_16), (unsigned char)(ipv4_addr.s_addr >> BITS_TO_SHIFT_24));
    }
    else
    {
        printf("Invalid IPv4 Address: %s\n", ipv4_address_str);
    }

    // Convert IPv6 address from string to binary format (network byte order)
    if(inet_pton(AF_INET6, ipv6_address_str, &ipv6_addr) == 1)
    {
        // Print the IPv6 address as an array of unsigned bytes
        const unsigned char *bytes = (unsigned char *)&ipv6_addr;

        printf("IPv6 Address: ");

        for(size_t i = 0; i < sizeof(ipv6_addr); i++)
        {
            printf("%02x", bytes[i]);
            if(i < sizeof(ipv6_addr) - 1)
            {
                printf(":");
            }
        }

        printf("\n");
    }
    else
    {
        printf("Invalid IPv6 Address: %s\n", ipv6_address_str);
    }

    return EXIT_SUCCESS;
}

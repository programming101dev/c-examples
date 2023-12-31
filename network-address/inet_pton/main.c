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

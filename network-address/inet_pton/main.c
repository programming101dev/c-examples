#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>


int main(void)
{
    const char *ipv4_address_str = "192.168.0.1";
    const char *ipv6_address_str = "2001:0db8:85a3:0000:0000:8a2e:0370:7334";

    struct in_addr ipv4_addr;
    struct in6_addr ipv6_addr;

    // Convert IPv4 address from string to binary format (network byte order)
    if(inet_pton(AF_INET, ipv4_address_str, &ipv4_addr) == 1)
    {
        printf("IPv4 Address: %u.%u.%u.%u\n",
               (unsigned char)ipv4_addr.s_addr,
               (unsigned char)(ipv4_addr.s_addr >> 8),
               (unsigned char)(ipv4_addr.s_addr >> 16),
               (unsigned char)(ipv4_addr.s_addr >> 24));
    }
    else
    {
        printf("Invalid IPv4 Address: %s\n", ipv4_address_str);
    }

    // Convert IPv6 address from string to binary format (network byte order)
    if(inet_pton(AF_INET6, ipv6_address_str, &ipv6_addr) == 1)
    {
        // Print the IPv6 address as an array of unsigned bytes
        unsigned char* bytes = (unsigned char*)&ipv6_addr;
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

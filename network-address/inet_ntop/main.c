#include <stdio.h>
#include <arpa/inet.h>


int main(void)
{
    struct in_addr ipv4_addr;

    // Hard-coded binary representations of an IPv4 address and an IPv6 address
    unsigned char ipv4_bytes[4] = {192, 168, 0, 1};
    unsigned char ipv6_bytes[16] = {32, 1, 13, 184, 133, 163, 0, 0, 0, 0, 138, 46, 3, 112, 115, 52};

    // Convert IPv4 address from binary format to presentation format (string)
    ipv4_addr.s_addr = *((unsigned int*)ipv4_bytes);
    char ipv4_str[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &ipv4_addr, ipv4_str, sizeof(ipv4_str));
    printf("IPv4 Address: %s\n", ipv4_str);

    // Convert IPv6 address from binary format to presentation format (string)
    char ipv6_str[INET6_ADDRSTRLEN];
    inet_ntop(AF_INET6, ipv6_bytes, ipv6_str, sizeof(ipv6_str));
    printf("IPv6 Address: %s\n", ipv6_str);

    return 0;
}

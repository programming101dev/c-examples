#include <stdio.h>
#include <arpa/inet.h>


int main(void)
{
    unsigned long network_value = 0x78563412; // Hex value of 0x12345678 in network byte order
    unsigned long host_value = ntohl(network_value);

    printf("Network value: %lu\n", network_value);
    printf("Host value: %lu\n", host_value);

    return 0;
}

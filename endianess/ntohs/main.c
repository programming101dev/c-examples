#include <stdio.h>
#include <arpa/inet.h>


int main(void)
{
    unsigned short network_value = 0x3039; // Hex value of 12345 in network byte order
    unsigned short host_value = ntohs(network_value);

    printf("Network value: %hu\n", network_value);
    printf("Host value: %hu\n", host_value);

    return 0;
}

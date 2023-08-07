#include <stdio.h>
#include <arpa/inet.h>


int main(void)
{
    unsigned long host_value = 0x12345678;
    unsigned long network_value = htonl(host_value);

    printf("Host value: %lu\n", host_value);
    printf("Network value: %lu\n", network_value);

    return 0;
}

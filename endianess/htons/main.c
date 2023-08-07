#include <stdio.h>
#include <arpa/inet.h>


int main(void)
{
    unsigned short host_value = 12345;
    unsigned short network_value = htons(host_value);

    printf("Host value: %hu\n", host_value);
    printf("Network value: %hu\n", network_value);

    return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <sys/utsname.h>


int main(void)
{
    struct utsname system_info;

    if(uname(&system_info) != 0)
    {
        perror("uname");
        return EXIT_FAILURE;
    }

    printf("System Name: %s\n", system_info.sysname);
    printf("Node Name: %s\n", system_info.nodename);
    printf("Release: %s\n", system_info.release);
    printf("Version: %s\n", system_info.version);
    printf("Machine: %s\n", system_info.machine);

    return EXIT_FAILURE;
}

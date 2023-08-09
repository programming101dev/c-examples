#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    char buffer[128];
    FILE *fp = popen("ls -l 2>&1", "r");

    if(fp == NULL)
    {
        perror("Error opening pipe");
        return 1;
    }

    printf("Output of \"ls -l\":\n");

    while(fgets(buffer, sizeof(buffer), fp) != NULL)
    {
        printf("%s", buffer);
    }

    if(pclose(fp) == -1)
    {
        perror("Error closing pipe");
        return 1;
    }

    return EXIT_SUCCESS;
}

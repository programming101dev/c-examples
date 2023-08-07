#include <stdio.h>
#include <stdlib.h>


int main(void)
{
    char *terminal_name = ctermid(NULL);

    if(*terminal_name != '\0') {
        printf("The current terminal name is: %s\n", terminal_name);
    } else {
        printf("Failed to get the current terminal name.\n");
    }

    return EXIT_SUCCESS;
}

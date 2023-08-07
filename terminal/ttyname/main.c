#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


int main(void) {
    char *tty = ttyname(STDIN_FILENO);

    if(tty != NULL) {
        printf("The terminal associated with stdin is: %s\n", tty);
    } else {
        printf("stdin is not associated with a terminal.\n");
    }

    return EXIT_SUCCESS;
}

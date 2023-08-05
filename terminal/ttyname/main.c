#include <stdio.h>
#include <unistd.h>

int main() {
    char *tty = ttyname(STDIN_FILENO);

    if (tty != NULL) {
        printf("The terminal associated with stdin is: %s\n", tty);
    } else {
        printf("stdin is not associated with a terminal.\n");
    }

    return 0;
}

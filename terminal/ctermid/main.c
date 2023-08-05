#include <stdio.h>

int main() {
    char *terminal_name = ctermid(NULL);

    if (*terminal_name != '\0') {
        printf("The current terminal name is: %s\n", terminal_name);
    } else {
        printf("Failed to get the current terminal name.\n");
    }

    return 0;
}

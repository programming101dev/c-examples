#include <stdio.h>
#include <stdlib.h>

int main(void) {
    // Execute "ls -l" command using system
    int status = system("ls -l");

    if(status == -1) {
        perror("Error executing command");
        return 1;
    } else {
        printf("Command exited with status: %d\n", status);
    }

    return EXIT_SUCCESS;
}

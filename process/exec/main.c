#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


int main(void)
{
    pid_t pid = getpid();
    printf("Current Process ID: %d\n", pid);

    // Execute the "ls -l" command using execl
    // The first argument is the path to the command (in this case, "ls")
    // The subsequent arguments are the command-line arguments to "ls" (in this case, "-l" for long format listing)
    if(execl("/bin/ls", "ls", "-l", (char *)NULL) == -1)
    {
        perror("execl");
        exit(EXIT_FAILURE);
    }

    // The code below this line will not be executed, as the process image is replaced by the new program.
    // However, it's a good practice to include it for completeness.

    printf("This line will not be executed if execl is successful.\n");

    return EXIT_SUCCESS;
}

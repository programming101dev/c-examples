#include <stdio.h>
#include <termios.h>
#include <unistd.h>

int main() {
    struct termios options;
    tcgetattr(STDOUT_FILENO, &options);
    cfsetispeed(&options, B9600);
    cfsetospeed(&options, B9600);
    options.c_cflag |= (CLOCAL | CREAD);
    tcsetattr(STDOUT_FILENO, TCSANOW, &options);

    char message[256];
    printf("Enter your message: ");
    fflush(stdout);

    // Read the message from stdin
    fgets(message, sizeof(message), stdin);

    // Write the message to stdout (which will be the terminal)
    printf("You wrote: %s\n", message);
    fflush(stdout);

    // Wait until all data is transmitted
    if (tcdrain(STDOUT_FILENO) != 0)
    {
        perror("Error waiting for data to be transmitted");
        return 1;
    }

    return 0;
}

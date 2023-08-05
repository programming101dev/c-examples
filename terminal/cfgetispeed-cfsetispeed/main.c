#include <stdio.h>
#include <termios.h>
#include <unistd.h>

int main(void) {
    struct termios term;
    speed_t input_baud_rate;

    // Get current terminal settings
    if (tcgetattr(STDIN_FILENO, &term) != 0) {
        perror("tcgetattr failed");
        return 1;
    }

    // Get current input baud rate
    input_baud_rate = cfgetispeed(&term);
    printf("Current input baud rate: %d\n", (int)input_baud_rate);

    // Set input baud rate to 9600
    if (cfsetispeed(&term, B9600) != 0) {
        perror("cfsetispeed failed");
        return 1;
    }

    // Update terminal settings
    if (tcsetattr(STDIN_FILENO, TCSANOW, &term) != 0) {
        perror("tcsetattr failed");
        return 1;
    }

    // Get updated input baud rate
    input_baud_rate = cfgetispeed(&term);
    printf("Updated input baud rate: %d\n", (int)input_baud_rate);

    return 0;
}

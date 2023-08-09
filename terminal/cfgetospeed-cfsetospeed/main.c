#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>

int main(void)
{
    struct termios term;

    // Get the terminal attributes
    if(tcgetattr(STDIN_FILENO, &term) == -1)
    {
        perror("tcgetattr");
        return 1;
    }

    // Get the current output baud rate
    speed_t output_baud = cfgetospeed(&term);

    if(output_baud == (speed_t)-1)
    {
        perror("cfgetospeed");
        return 1;
    }

    printf("Current output baud rate: %lu\n", output_baud);

    // Set a new output baud rate
    speed_t new_output_baud = B9600; // 9600 baud

    if(cfsetospeed(&term, new_output_baud) == -1)
    {
        perror("cfsetospeed");
        return 1;
    }

    printf("New output baud rate: %lu\n", new_output_baud);

    // Apply the updated terminal attributes
    if(tcsetattr(STDIN_FILENO, TCSANOW, &term) == -1)
    {
        perror("tcsetattr");
        return 1;
    }

    return EXIT_SUCCESS;
}

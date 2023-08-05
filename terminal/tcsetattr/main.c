#include <stdio.h>
#include <termios.h>
#include <unistd.h>

int main() {
    struct termios term;
    struct termios original_term;

    // Get the current terminal settings
    if (tcgetattr(STDIN_FILENO, &term) != 0) {
        perror("tcgetattr");
        return 1;
    }

    // Save the original terminal settings
    original_term = term;

    // Modify the terminal attributes to turn off ECHO and enable non-canonical mode
    term.c_lflag &= ~(ICANON | ECHO);

    // Set the minimum number of bytes to read for non-canonical mode (1 byte at a time)
    term.c_cc[VMIN] = 1;
    term.c_cc[VTIME] = 0;

    // Apply the new terminal settings
    if (tcsetattr(STDIN_FILENO, TCSANOW, &term) != 0) {
        perror("tcsetattr");
        return 1;
    }

    char ch;

    // Read the first character without echoing
    printf("Type the first character: ");
    if (read(STDIN_FILENO, &ch, 1) == 1) {
        printf("%c\n", ch);
    }

    // Restore the original terminal settings with ECHO turned on and canonical mode
    term = original_term;
    if (tcsetattr(STDIN_FILENO, TCSANOW, &term) != 0) {
        perror("tcsetattr");
        return 1;
    }

    // Read the rest of the characters with echoing
    printf("Type the rest of the characters: ");
    while (1) {
        if (read(STDIN_FILENO, &ch, 1) == 1) {
            if (ch == '\n') {
                break;
            }
            printf("%c", ch);
            fflush(stdout); // Force printing without newline
        }
    }
    printf("\n");

    return 0;
}

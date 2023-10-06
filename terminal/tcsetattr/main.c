/*
 * This code is licensed under the Attribution-NonCommercial-NoDerivatives 4.0 International license.
 *
 * Author: D'Arcy Smith (ds@programming101.dev)
 *
 * You are free to:
 *   - Share: Copy and redistribute the material in any medium or format.
 *   - Under the following terms:
 *       - Attribution: You must give appropriate credit, provide a link to the license, and indicate if changes were made.
 *       - NonCommercial: You may not use the material for commercial purposes.
 *       - NoDerivatives: If you remix, transform, or build upon the material, you may not distribute the modified material.
 *
 * For more details, please refer to the full license text at:
 * https://creativecommons.org/licenses/by-nc-nd/4.0/
 */

#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    struct termios term;
    struct termios original_term;
    int            ch;

    // Get the current terminal settings
    if(tcgetattr(STDIN_FILENO, &term) != 0)
    {
        perror("tcgetattr");
        return EXIT_FAILURE;
    }

    // Save the original terminal settings
    original_term = term;

    // Modify the terminal attributes to turn off ECHO and enable non-canonical mode
    term.c_lflag &= ~(tcflag_t)(ICANON | ECHO);

    // Set the minimum number of bytes to read for non-canonical mode (1 byte at a time)
    term.c_cc[VMIN]  = 1;
    term.c_cc[VTIME] = 0;

    // Apply the new terminal settings
    if(tcsetattr(STDIN_FILENO, TCSANOW, &term) != 0)
    {
        perror("tcsetattr");
        return EXIT_FAILURE;
    }

    // Read the first character without echoing
    printf("Type the first character: ");
    fflush(stdout);

    if(read(STDIN_FILENO, &ch, 1) == 1)
    {
        printf("%c\n", ch);
    }

    // Restore the original terminal settings with ECHO turned on and canonical mode
    term = original_term;

    if(tcsetattr(STDIN_FILENO, TCSANOW, &term) != 0)
    {
        perror("tcsetattr");
        return EXIT_FAILURE;
    }

    // Read the rest of the characters with echoing
    printf("Type the rest of the characters: ");
    fflush(stdout);

    while(1)
    {
        if(read(STDIN_FILENO, &ch, 1) == 1)
        {
            if(ch == '\n')
            {
                break;
            }

            printf("%c", (char)ch);
            fflush(stdout);    // Force printing without newline
        }
    }

    printf("\n");

    return EXIT_SUCCESS;
}

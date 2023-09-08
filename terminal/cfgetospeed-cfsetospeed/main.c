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


// TODO: pass the new baud on the commadn line


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

    if(output_baud == (speed_t) - 1)
    {
        perror("cfgetospeed");
        return 1;
    }

    printf("Current output baud rate: %lu\n", (unsigned long) output_baud);

    // Set a new output baud rate
    speed_t new_output_baud = B9600; // 9600 baud

    if(cfsetospeed(&term, new_output_baud) == -1)
    {
        perror("cfsetospeed");
        return 1;
    }

    printf("New output baud rate: %lu\n", (unsigned long) new_output_baud);

    // Apply the updated terminal attributes
    if(tcsetattr(STDIN_FILENO, TCSANOW, &term) == -1)
    {
        perror("tcsetattr");
        return 1;
    }

    return EXIT_SUCCESS;
}

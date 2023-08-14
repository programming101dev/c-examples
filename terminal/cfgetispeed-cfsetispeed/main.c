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


int main(void)
{
    struct termios term;
    speed_t input_baud_rate;

    // Get current terminal settings
    if(tcgetattr(STDIN_FILENO, &term) != 0)
    {
        perror("tcgetattr failed");
        return 1;
    }

    // Get current input baud rate
    input_baud_rate = cfgetispeed(&term);
    printf("Current input baud rate: %d\n", (int) input_baud_rate);

    // Set input baud rate to 9600
    if(cfsetispeed(&term, B9600) != 0)
    {
        perror("cfsetispeed failed");
        return 1;
    }

    // Update terminal settings
    if(tcsetattr(STDIN_FILENO, TCSANOW, &term) != 0)
    {
        perror("tcsetattr failed");
        return 1;
    }

    // Get updated input baud rate
    input_baud_rate = cfgetispeed(&term);
    printf("Updated input baud rate: %d\n", (int) input_baud_rate);

    return EXIT_SUCCESS;
}

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


#define MESSAGE_LEN 256


int main(void)
{
    struct termios options;
    char           message[MESSAGE_LEN];

    tcgetattr(STDOUT_FILENO, &options);
    cfsetispeed(&options, B9600);
    cfsetospeed(&options, B9600);
    options.c_cflag |= (CLOCAL | CREAD);
    tcsetattr(STDOUT_FILENO, TCSANOW, &options);
    printf("Enter your message: ");
    fflush(stdout);

    // Read the message from stdin
    fgets(message, sizeof(message), stdin);

    // Write the message to stdout (which will be the terminal)
    printf("You wrote: %s\n", message);
    fflush(stdout);

    // Wait until all data is transmitted
    if(tcdrain(STDOUT_FILENO) != 0)
    {
        perror("Error waiting for data to be transmitted");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

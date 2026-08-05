/*
 * Copyright 2026 D'Arcy Smith
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>

int main(void)
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

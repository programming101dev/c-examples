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


#include "testlib.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


void display(const char *msg)
{
    char *uppercase_msg = strdup(msg);

    if(uppercase_msg == NULL)
    {
        fprintf(stderr, "Error: Memory allocation failed.\n");
        return;
    }

    // Convert the message to uppercase
    for(int i = 0; uppercase_msg[i]; i++)
    {
        uppercase_msg[i] = (char)toupper(uppercase_msg[i]);
    }

    printf("testlib-2: display: %s\n", uppercase_msg);

    free(uppercase_msg);
}

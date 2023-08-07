#include "testlib.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


void display(const char *msg)
{
    char *uppercase_msg = strdup(msg);
    if (uppercase_msg == NULL)
    {
        fprintf(stderr, "Error: Memory allocation failed.\n");
        return;
    }

    // Convert the message to uppercase
    for (int i = 0; uppercase_msg[i]; i++)
    {
        uppercase_msg[i] = toupper(uppercase_msg[i]);
    }

    printf("testlib-2: display: %s\n", uppercase_msg);

    free(uppercase_msg);
}
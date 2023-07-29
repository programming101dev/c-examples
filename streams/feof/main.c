#include <stdio.h>
#include <stdlib.h>


const int BUFFER_SIZE = 256;


int main(void)
{
    char buffer[BUFFER_SIZE];
    printf("Enter some text (press Ctrl+D on Unix/Linux or Ctrl+Z on Windows to stop):\n");

    while(!feof(stdin))
    {
        if(fgets(buffer, BUFFER_SIZE, stdin) != NULL)
        {
            printf("You entered: %s", buffer);
        }
    }

    printf("\nReached the end of input.\n");

    return EXIT_SUCCESS;
}

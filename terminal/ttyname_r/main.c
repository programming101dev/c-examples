#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>


int main(void)
{
    long max_name_length;

    max_name_length = sysconf(_SC_TTY_NAME_MAX);

    if(max_name_length == -1)
    {
        perror("Error getting maximum terminal name length");
        return 1;
    }

    char ttyname_buf[max_name_length + 1];

    if(ttyname_r(STDIN_FILENO, ttyname_buf, sizeof(ttyname_buf)) == 0)
    {
        printf("The terminal associated with stdin is: %s\n", ttyname_buf);
    }
    else
    {
        perror("stdin is not associated with a terminal");
    }

    return EXIT_SUCCESS;
}

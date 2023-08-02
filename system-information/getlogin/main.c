#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


int main(void)
{
    char *login_name;

    login_name = getlogin();

    if(login_name != NULL)
    {
        printf("Logged-in user: %s\n", login_name);
    }
    else
    {
        perror("getlogin");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

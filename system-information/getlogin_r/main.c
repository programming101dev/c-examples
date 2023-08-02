#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


int main(void)
{
    long max_login_length;
    char *login_name;
    int result;

    max_login_length = sysconf(_SC_LOGIN_NAME_MAX);

    if (max_login_length <= 0)
    {
        perror("sysconf");

        return EXIT_FAILURE;
    }

    login_name = malloc(max_login_length + 1);

    if(login_name == NULL)
    {
        perror("malloc");

        return EXIT_FAILURE;
    }

    result = getlogin_r(login_name, max_login_length);

    if(result == 0)
    {
        printf("Logged-in user: %s\n", login_name);
    }
    else
    {
        perror("getlogin_r");
        free(login_name);

        return EXIT_FAILURE;
    }

    free(login_name);

    return EXIT_SUCCESS;
}

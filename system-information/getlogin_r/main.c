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
#include <unistd.h>


int main(void)
{
    long max_login_length;
    char *login_name;
    int result;

    max_login_length = sysconf(_SC_LOGIN_NAME_MAX);

    if(max_login_length <= 0) // TODO: can I handle this differently, like with getcwd?
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

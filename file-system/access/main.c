#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>


static void check_file_access(const char *filename, int mode, const char *access_msg);


int main(void)
{
    const char *filename = "../../example.txt";

    check_file_access(filename, R_OK, "readable");
    check_file_access(filename, W_OK, "writable");
    check_file_access(filename, X_OK, "executable");

    return EXIT_SUCCESS;
}


static void check_file_access(const char *filename, int mode, const char *access_msg)
{
    if(access(filename, mode) == 0)
    {
        printf("%s is %s.\n", filename, access_msg);
    }
    else
    {
        if(errno == EACCES)
        {
            printf("Access denied: %s is not %s.\n", filename, access_msg);
        }
        else
        {
            perror("Error accessing file");
        }

        exit(EXIT_FAILURE);
    }
}
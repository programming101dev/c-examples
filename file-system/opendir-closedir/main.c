#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>


int main(void)
{
    DIR *dir = opendir(".");

    if(dir == NULL)
    {
        perror("opendir");
        return EXIT_FAILURE;
    }

    // Do something with the directory...

    if(closedir(dir) == -1)
    {
        perror("closedir");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

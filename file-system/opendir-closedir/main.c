#include <stdio.h>
#include <dirent.h>

int main() {
    DIR *dir = opendir(".");

    if (dir == NULL)
    {
        perror("opendir");
        return 1;
    }

    // Do something with the directory...

    if(closedir(dir) == -1)
    {
        perror("closedir");
        return 1;
    }

    return 0;
}

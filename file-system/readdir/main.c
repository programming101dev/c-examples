#include <stdio.h>
#include <dirent.h>


int main(void)
{
    DIR *dir = opendir(".");
    if(dir == NULL)
    {
        perror("opendir");
        return 1;
    }

    struct dirent *entry;
    while((entry = readdir(dir)) != NULL)
    {
        printf("Name: %s\n", entry->d_name);
#if defined(__APPLE__)
        printf("Inode number: %lld\n", entry->d_ino);
#else
        printf("Inode number: %ld\n", entry->d_ino);
#endif
        printf("\n");
    }

    if(closedir(dir) == -1)
    {
        perror("closedir");
        return 1;
    }

    return 0;
}

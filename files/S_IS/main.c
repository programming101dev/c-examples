#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>


int main(int argc, char *argv[])
{
    const char *filename = argv[1];
    struct stat fileStat;

    if(argc != 2)
    {
        printf("Usage: %s <filename>\n", argv[0]);
        return EXIT_FAILURE;
    }

    if(stat(filename, &fileStat) == -1)
    {
        perror("Error getting file stats");
        return EXIT_FAILURE;
    }

    printf("File Information for '%s':\n\n", filename);

    if(S_ISBLK(fileStat.st_mode))
    {
        printf("Type: Block special file\n");
    }
    else if(S_ISCHR(fileStat.st_mode))
    {
        printf("Type: Character special file\n");
    }
    else if(S_ISDIR(fileStat.st_mode))
    {
        printf("Type: Directory\n");
    }
    else if(S_ISFIFO(fileStat.st_mode))
    {
        printf("Type: FIFO/Named Pipe\n");
    }
    else if(S_ISREG(fileStat.st_mode))
    {
        printf("Type: Regular file\n");
    }
    else if(S_ISLNK(fileStat.st_mode))
    {
        printf("Type: Symbolic link\n");
    }
    else if(S_ISSOCK(fileStat.st_mode))
    {
        printf("Type: Socket\n");
    }
    else
    {
        printf("Type: Unknown file type\n");
    }

    if(S_TYPEISMQ(&fileStat))
    {
        printf("Type: Message Queue\n");
    }
    else if(S_TYPEISSEM(&fileStat))
    {
        printf("Type: Semaphore\n");
    }
    else if(S_TYPEISSHM(&fileStat))
    {
        printf("Type: Shared Memory\n");
    }

    printf("\n");
    printf("File Size: %lld bytes\n", (long long)fileStat.st_size);
    printf("File Permissions: %o\n", fileStat.st_mode & (S_IRWXU | S_IRWXG | S_IRWXO));
    printf("File inode: %lld\n", (long long)fileStat.st_ino);
    printf("Device ID: %lld\n", (long long)fileStat.st_dev);
    printf("Number of hard links: %lld\n", (long long)fileStat.st_nlink);
    printf("File Owner UID: %d\n", fileStat.st_uid);
    printf("File Group GID: %d\n", fileStat.st_gid);
    printf("Last access time: %ld\n", fileStat.st_atime);
    printf("Last modification time: %ld\n", fileStat.st_mtime);
    printf("Last status change time: %ld\n", fileStat.st_ctime);

    return 0;
}

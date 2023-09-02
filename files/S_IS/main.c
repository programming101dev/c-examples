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


#include <fcntl.h>
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>


static void usage(const char *program_name, int exit_code, const char *message);
static void print_file_info(const struct stat *fileStat);
static void print_special_type(const struct stat *fileStat);
static void print_extended_type(const struct stat *fileStat);
static void print_permissions(mode_t mode);
static void print_time(const char *label, time_t timeValue);


int main(int argc, char *argv[])
{
    const char *filename = NULL;
    struct stat fileStat;
    int opt;

    while ((opt = getopt(argc, argv, "hf:")) != -1)
    {
        switch (opt)
        {
            case 'f':
            {
                filename = optarg;
                break;
            }
            case 'h':
            {
                usage(argv[0], EXIT_SUCCESS, NULL);
                break;
            }
            case '?':
            {
                char message[24];

                snprintf(message, sizeof(message), "Unknown option '-%c'.\n", optopt);
                usage(argv[0], EXIT_FAILURE, message);
                break;
            }
            default:
            {
                usage(argv[0], EXIT_FAILURE, NULL);
            }
        }
    }

    if(filename == NULL)
    {
        usage(argv[0], EXIT_FAILURE, "Error: You must provide a filename using -f option.\n");
    }

    if(stat(filename, &fileStat) == -1)
    {
        perror("Error getting file stats");
        return EXIT_FAILURE;
    }

    printf("File Information for '%s':\n\n", filename);
    print_file_info(&fileStat);

    return EXIT_SUCCESS;
}


static void usage(const char *program_name, int exit_code, const char *message)
{
    if(message)
    {
        fputs(message, stderr);
    }

    fprintf(stderr, "Usage: %s <filename>\n", program_name);
    exit(exit_code);
}


static void print_file_info(const struct stat *fileStat)
{
    print_special_type(fileStat);
    print_extended_type(fileStat);
    printf("File Size: %lld bytes\n", (long long)fileStat->st_size);
    print_permissions(fileStat->st_mode);
    printf("File inode: %lld\n", (long long)fileStat->st_ino);
    printf("Device ID: %lld\n", (long long)fileStat->st_dev);
    printf("Number of hard links: %lld\n", (long long)fileStat->st_nlink);
    printf("File Owner UID: %d\n", fileStat->st_uid);
    printf("File Group GID: %d\n", fileStat->st_gid);
    print_time("Last access time", fileStat->st_atime);
    print_time("Last modification time", fileStat->st_mtime);
    print_time("Last status change time", fileStat->st_ctime);
}


static void print_special_type(const struct stat *fileStat)
{
    if (S_ISBLK(fileStat->st_mode))
    {
        printf("Type: Block special file\n");
    }
    else if (S_ISCHR(fileStat->st_mode))
    {
        printf("Type: Character special file\n");
    }
    else if (S_ISDIR(fileStat->st_mode))
    {
        printf("Type: Directory\n");
    }
    else if (S_ISFIFO(fileStat->st_mode))
    {
        printf("Type: FIFO/Named Pipe\n");
    }
    else if (S_ISREG(fileStat->st_mode))
    {
        printf("Type: Regular file\n");
    }
    else if (S_ISLNK(fileStat->st_mode))
    {
        printf("Type: Symbolic link\n");
    }
    else if (S_ISSOCK(fileStat->st_mode))
    {
        printf("Type: Socket\n");
    }
    else
    {
        printf("Type: Unknown file type\n");
    }
}


static void print_extended_type(const struct stat *fileStat)
{
    // compiler trick - on macOS these all return 0 so fileStat is unused
    (void)fileStat;

    if(S_TYPEISMQ(fileStat))
    {
        printf("Type: Message Queue\n");
    }
    else if(S_TYPEISSEM(fileStat))
    {
        printf("Type: Semaphore\n");
    }
    else if(S_TYPEISSHM(fileStat))
    {
        printf("Type: Shared Memory\n");
    }
}


static void print_permissions(mode_t mode)
{
    printf("File Permissions: %o\n", mode & (S_IRWXU | S_IRWXG | S_IRWXO));
}


static void print_time(const char *label, time_t timeValue)
{
    printf("%s: %ld\n", label, timeValue);
}



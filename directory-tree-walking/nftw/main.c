#define _XOPEN_SOURCE 500
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <ftw.h>

static int print_file(const char* fpath, const struct stat* sb, int tflag, struct FTW* ftwbuf) {
    if (tflag == FTW_F)
        printf("File: %s\n", fpath);
    else if (tflag == FTW_D)
        printf("Directory: %s\n", fpath);
    else if (tflag == FTW_SL)
        printf("Link: %s\n", fpath);

    return 0; // Continue traversing the directory tree
}

void display_help(const char* program_name) {
    printf("Usage: %s <directory>\n", program_name);
}

int main(int argc, char* argv[]) {
    int opt;
    while ((opt = getopt(argc, argv, "h")) != -1)
    {
        switch (opt) {
            case 'h':
                display_help(argv[0]);
                return 0;
            default:
                display_help(argv[0]);
                return 1;
        }
    }

    if (optind != argc - 1)
    {
        display_help(argv[0]);
        return 1;
    }

    // Use nftw to traverse the directory tree recursively
    if (nftw(argv[optind], print_file, 10, FTW_PHYS) == -1)
    {
        perror("nftw");
        return 1;
    }

    return 0;
}

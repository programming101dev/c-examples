#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>


int main(void)
{
    const char *tempdir_template = "example_dir";

    if(mkdir(tempdir_template, 0700) == -1)
    {
        perror("Error creating temporary directory");
        return 1;
    }

    printf("Directory created: %s\n", tempdir_template);

    // Remove the temporary directory
    if(rmdir(tempdir_template) == -1)
    {
        perror("Error removing temporary directory");
        return EXIT_FAILURE;
    }

    printf("Temporary directory removed: %s\n", tempdir_template);
    return EXIT_SUCCESS;
}

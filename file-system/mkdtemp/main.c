#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


int main(void)
{
    char tempdir_template[] = "exampleXXXXXX";

    if(mkdtemp(tempdir_template) == NULL)
    {
        perror("Error creating temporary directory");
        return EXIT_FAILURE;
    }

    printf("Temporary directory created: %s\n", tempdir_template);

    if(rmdir(tempdir_template) == -1)
    {
        perror("Error removing temporary directory");
        return 1;
    }

    printf("Temporary directory removed: %s\n", tempdir_template);

    return EXIT_SUCCESS;
}

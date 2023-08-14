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
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


int main(void)
{
    int fd;
    FILE *file;
    char buffer[256];

    fd = open("../../example.txt", O_RDONLY);

    if(fd == -1)
    {
        perror("Error opening file");
        return EXIT_FAILURE;
    }

    file = fdopen(fd, "r");

    if(file == NULL)
    {
        perror("Error creating FILE* stream");
        close(fd);
        return EXIT_FAILURE;
    }

    while(fgets(buffer, sizeof(buffer), file) != NULL)
    {
        printf("%s", buffer); // Print the content to stdout
    }

    if(fclose(file) == EOF)
    {
        perror("Error closing FILE* stream");
        return EXIT_FAILURE;
    }

    // close was called by fclose

    return EXIT_SUCCESS;
}

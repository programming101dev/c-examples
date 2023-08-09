#include <stdio.h>
#include <stdlib.h>


static void display_file(FILE *file, const char *message);


int main(void)
{
    FILE *file;

    file = fopen("../../example.txt", "r");

    if(file == NULL)
    {
        perror("Error opening the file");
        return EXIT_FAILURE;
    }

    display_file(file, "File contents:\n\n");
    fseeko(file, 0L, SEEK_SET);
    display_file(file, "\n\nFile contents after SEEK_SET:\n\n");
    fseeko(file, -10L, SEEK_CUR);
    display_file(file, "\n\nFile contents after SEEK_CUR -10:\n\n");
    fclose(file);
    return EXIT_SUCCESS;
}


static void display_file(FILE *file, const char *message)
{
    char ch;

    fputs(message, stdout);

    while((ch = fgetc(file)) != EOF)
    {
        putchar(ch);
    }
}

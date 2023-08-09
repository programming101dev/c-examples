#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

static void print_binary(int ch);

int main(void)
{
    printf("Char | Binary  | Oct | Dec | Hex | alnum | alpha | blank | cntrl | digit | graph | lower | print | punct | space | upper | xdigit | lower | upper |\n");
    printf("---------------------------------------------------------------------------------------------------------------------------------------------------\n");

    for(int ch = 0; ch <= 127; ch++)
    {
        printf("%c    | ", isprint(ch) ? ch : ' ');
        print_binary(ch);
        printf(" | %3o | %3d | %3X |   %d   |   %d   |   %d   |   %d   |   %d   |   %d   |   %d   |   %d   |   %d   |   %d   |   %d   |   %d    |   %c   |   %c   |\n",
               ch,
               ch,
               ch,
               isalnum(ch),
               isalpha(ch),
               isblank(ch),
               iscntrl(ch),
               isdigit(ch),
               isgraph(ch),
               islower(ch),
               isprint(ch),
               ispunct(ch),
               isspace(ch),
               isupper(ch),
               isxdigit(ch),
               islower(ch) ? tolower(ch) : ' ',   // Print lowercase character for tolower, otherwise print space.
               isupper(ch) ? toupper(ch) : ' '    // Print uppercase character for toupper, otherwise print space.
        );
    }

    return EXIT_SUCCESS;
}

static void print_binary(int ch)
{
    for(int i = 6; i >= 0; i--)
    {
        printf("%d", (ch >> i) & 1);
    }
}

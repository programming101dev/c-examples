#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define FIFO_FILE "../fifo_example"

int main(void)
{
    int fd;
    char line[1024]; // Adjust the buffer size as needed

    // Open the FIFO for writing
    fd = open(FIFO_FILE, O_WRONLY);
    if(fd == -1) {
        perror("open");
        exit(EXIT_FAILURE);
    }

    // Open the file to read
    FILE *file = fopen("../example.txt", "r");
    if(file == NULL) {
        perror("fopen");
        close(fd);
        exit(EXIT_FAILURE);
    }

    // Read and parse words from the file
    while(fgets(line, sizeof(line), file) != NULL) {
        char *word;
        word = strtok(line, " \t\n");
        while(word != NULL) {
            size_t word_len = strlen(word);
            if(word_len > UINT8_MAX) {
                fprintf(stderr, "Word exceeds maximum length\n");
                fclose(file);
                close(fd);
                exit(EXIT_FAILURE);
            }

            // Write the size of the word as uint8_t
            uint8_t size = (uint8_t)word_len;
            write(fd, &size, sizeof(uint8_t));

            // Write the word
            write(fd, word, word_len);
            word = strtok(NULL, " \t\n");
        }
    }

    fclose(file);
    close(fd);
    return EXIT_SUCCESS;
}

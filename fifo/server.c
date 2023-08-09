#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

#define FIFO_FILE "../fifo_example"

int main(void)
{
    int fd;
    uint8_t size;
    char word[UINT8_MAX + 1];

    // Create the FIFO if it doesn't exist
    mkfifo(FIFO_FILE, 0666);

    // Open the FIFO for reading
    fd = open(FIFO_FILE, O_RDONLY);
    if(fd == -1) {
        perror("open");
        exit(EXIT_FAILURE);
    }

    // Read and print words from the client
    while(read(fd, &size, sizeof(uint8_t)) > 0) {
        read(fd, word, size);
        word[size] = '\0'; // Null-terminate the string
        printf("Word Size: %u, Word: %s\n", size, word);
    }

    close(fd);
    return EXIT_SUCCESS;
}

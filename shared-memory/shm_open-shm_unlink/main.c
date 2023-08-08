#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

int main() {
    const char* shm_name = "/my_shared_memory";

    // Create a shared memory object
    int shm_fd = shm_open(shm_name, O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
    if (shm_fd == -1) {
        perror("shm_open");
        exit(1);
    }

    // Unlink the shared memory object
    if (shm_unlink(shm_name) == -1) {
        perror("shm_unlink");
        close(shm_fd);
        exit(1);
    }

    close(shm_fd);
    return 0;
}

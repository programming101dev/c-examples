#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
    pid_t pid = getpid(); // Get the PID of the current process

    pid_t sid = getsid(pid);
    if (sid == -1) {
        perror("Error getting SID");
        return 1;
    }

    printf("Process ID (PID): %d\n", pid);
    printf("Session ID (SID) of the process with PID %d: %d\n", pid, sid);

    return 0;
}

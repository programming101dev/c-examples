#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>


static void print_process_info(const char *name);


int main(void)
{
    pid_t pid;

    pid = fork();

    if(pid == -1)
    {
        perror("Error creating child process");
        return EXIT_FAILURE;
    }
    else if(pid == 0)
    {
        // This is the child process
        print_process_info("Child");
        printf("Child process finished.\n");
    }
    else
    {
        int status;

        // This is the parent process
        print_process_info("Parent");
        printf("Parent is waiting for the child\n");

        // Wait for the child process to finish
        if(waitpid(pid, &status, 0) == -1)
        {
            perror("Error waiting for child process");
            return EXIT_FAILURE;
        }

        printf("Parent process finished.\n");
    }

    return EXIT_SUCCESS;
}

static void print_process_info(const char *name)
{
    printf("Name: %s, Process: PID=%d, Parent PID=%d\n", name, getpid(), getppid());
}

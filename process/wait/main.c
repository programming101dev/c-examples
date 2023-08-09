#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>


static void print_process_info(const char *name);
static void child_process(const char *name);


int main(void)
{
    pid_t pid1;
    pid_t pid2;

    pid1 = fork();

    if(pid1 == -1)
    {
        perror("Error creating child process 1");
        return EXIT_FAILURE;
    }
    else if(pid1 == 0)
    {
        // This is the first child process
        child_process("Child 1");
        return EXIT_SUCCESS;
    }

    pid2 = fork();

    if(pid2 == -1)
    {
        perror("Error creating child process 2");
        return EXIT_FAILURE;
    }
    else if(pid2 == 0)
    {
        // This is the second child process
        child_process("Child 2");
        return EXIT_SUCCESS;
    }

    // This is the parent process
    print_process_info("Parent");

    // Wait for the child processes to finish
    int status1, status2;
    if(waitpid(pid1, &status1, 0) == -1)
    {
        perror("Error waiting for child process 1");
        return EXIT_FAILURE;
    }

    if(waitpid(pid2, &status2, 0) == -1)
    {
        perror("Error waiting for child process 2");
        return EXIT_FAILURE;
    }

    // Print exit status of child processes
    if(WIFEXITED(status1))
    {
        printf("Child 1 exited with status: %d\n", WEXITSTATUS(status1));
    }
    if(WIFEXITED(status2))
    {
        printf("Child 2 exited with status: %d\n", WEXITSTATUS(status2));
    }

    return EXIT_SUCCESS;
}

static void print_process_info(const char *name)
{
    printf("Name: %s, Process: PID=%d, Parent PID=%d\n", name, getpid(), getppid());
}

static void child_process(const char *name)
{
    srand(time(NULL) ^ getpid());
    print_process_info(name);
    int sleep_time = rand() % 5;
    sleep(sleep_time);
    printf("%s process finished after sleeping %d seconds.\n", name, sleep_time);
}

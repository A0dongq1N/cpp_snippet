#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>


int main() {
    pid_t cpid = fork();
    if (cpid == 0) {
        // Child process
        printf("Child process pid=%d\n", getpid());
        sleep(5); // Simulate some work
        exit(1);
    } else {
        // Parent process
        int status;
        wait(&status); // Wait for the child to finish
        printf("Child process exited with status %d\n", WEXITSTATUS(status));

        if (WIFEXITED(status)) {
            printf("Child process exited normally\n");
        } else {
            printf("Child process exited abnormally\n");
        }
    }

    return 0;
}
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>


int main() {
    pid_t fork_res = fork();
    if (fork_res == 0) {
        // Child process
        printf("Child process pid=%d\n", getpid());
        sleep(5); // Simulate some work
        exit(1);
    } else {
        // Parent process
        int status;
        waitpid(-1, &status, 0);
        // WEXITSTATUS(status) 返回子进程的退出状态
        printf("Child process exited with status %d\n", WEXITSTATUS(status));

        // WIFEXITED(status) 返回子进程是否正常退出
        if (WIFEXITED(status)) {
            printf("Child process exited normally\n");
        } else {
            printf("Child process exited abnormally\n");
        }
    }

    return 0;
}
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>

static void print_status(pid_t pid, int status) {
    if (WIFEXITED(status)) {
        printf("[pid=%d] 正常退出，exit code=%d\n", pid, WEXITSTATUS(status));
    } else if (WIFSIGNALED(status)) {
        printf("[pid=%d] 被信号终止，signal=%d\n", pid, WTERMSIG(status));
#ifdef WCOREDUMP
        if (WCOREDUMP(status)) printf(" (core dumped)\n");
#endif
    } else if (WIFSTOPPED(status)) {
        printf("[pid=%d] 被信号停止，signal=%d\n", pid, WSTOPSIG(status));
    } else if (WIFCONTINUED(status)) {
        printf("[pid=%d] 已继续运行（收到 SIGCONT）\n", pid);
    } else {
        printf("[pid=%d] 未知状态: 0x%x\n", pid, status);
    }
}

int main(void) {
    pid_t pid = fork();
    if (pid < 0) {
        perror("fork");
        return 1;
    } else if (pid == 0) {
        // 子进程：模拟工作一段时间，然后正常退出
        for (int i = 0; i < 5; ++i) {
            printf("child working... i=%d\n", i);
            fflush(stdout);
            sleep(1);
        }
        _exit(42);
    } else {
        // 父进程：1) 发送 SIGSTOP 并捕获“停止”状态
        sleep(1); // 给子进程一点时间启动
        if (kill(pid, SIGSTOP) == -1) {
            perror("kill(SIGSTOP)");
            return 1;
        }

        int status = 0;
        // WUNTRACED 的意思是：即使子进程没有退出，只是被信号停止，也要返回并设置 status 使得 WIFSTOPPED(status) 为真。
        pid_t r = waitpid(pid, &status, WUNTRACED);
        if (r == -1) {
            perror("waitpid(..., WUNTRACED)");
            return 1;
        }
        if (WIFSTOPPED(status)) {
            print_status(r, status);
        } else {
            printf("未捕获到 STOP 事件，状态: 0x%x\n", status);
        }

        // 2) 发送 SIGCONT 并捕获“继续运行”状态
        if (kill(pid, SIGCONT) == -1) {
            perror("kill(SIGCONT)");
            return 1;
        }

        status = 0;
        // WCONTINUED 的意思是：在子进程被停止后，如果它因为收到 SIGCONT 而继续运行，则返回一次并设置 status 使得 WIFCONTINUED(status) 为真。
        r = waitpid(pid, &status, WCONTINUED);
        if (r == -1) {
            perror("waitpid(..., WCONTINUED)");
            return 1;
        }
        if (WIFCONTINUED(status)) {
            print_status(r, status);
        } else {
            printf("未捕获到 CONTINUED 事件，状态: 0x%x\n", status);
        }

        // 3) 最后等待子进程正常退出并解析退出码
        status = 0;
        r = waitpid(pid, &status, 0); // 阻塞等待退出
        if (r == -1) {
            perror("waitpid(..., 0)");
            return 1;
        }
        print_status(r, status);
    }
    return 0;
}

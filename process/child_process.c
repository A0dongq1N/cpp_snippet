#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
//模拟父子进程执行顺序的程序
int main() {
    pid_t pid = fork();

    if (pid < 0) {
        perror("fork");
        return 1;
    } else if (pid == 0) {
        // 子进程
        for (int i = 1; i <= 10; i++) {
            printf("%d ", i);
        }
        printf("");
        sleep(5);
        return 0;
    } else {
        // 父进程
        int number;
        // printf("请输入一个数以等待子进程完成: ");
        // scanf("%d", &number);
        // printf("您输入的数是: %d", number);
        // printf("子进程已完成");
    }

    return 0;
}

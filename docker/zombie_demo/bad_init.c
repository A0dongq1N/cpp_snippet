/*
 * bad_init.c — Scene 1 PID 1：模拟一个不 reap 的业务进程
 *
 * 启动 zombie_factory + monitor.sh，自身永不调用 waitpid()
 * → zombie_factory 退出后，孤儿归 bad_init → 永久堆积
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/prctl.h>

int main(void) {
    prctl(PR_SET_CHILD_SUBREAPER, 1, 0, 0, 0);
    printf("[bad_init] PID=%d — I am PID 1, will NOT reap zombies\n", getpid());
    fflush(stdout);

    /* fork zombie_factory */
    if (fork() == 0) {
        execl("/app/zombie_factory", "zombie_factory", NULL);
        perror("exec zombie_factory"); exit(1);
    }

    /* fork monitor.sh */
    if (fork() == 0) {
        execl("/bin/bash", "bash", "/app/monitor.sh", NULL);
        perror("exec monitor.sh"); exit(1);
    }

    /* 永不 waitpid — 这是 bug */
    while (1) sleep(86400);
    return 0;
}

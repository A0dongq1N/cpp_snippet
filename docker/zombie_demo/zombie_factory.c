/*
 * zombie_factory.c
 *
 * fork 8 个立即退出的子进程（不调用 wait），然后自身在 4 秒后退出。
 *
 * 时序：
 *   t=0  fork child-1..8 → 子进程立即退出 → 成为 zombie_factory 的僵尸
 *   t=4  zombie_factory 退出 → 子进程孤儿被 PID 1 收养
 *        Scene 1: PID1=bad_init  → 不 reap → 僵尸永久堆积
 *        Scene 2: PID1=tini      → waitpid(-1) → 立即收割
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

#define NUM_CHILDREN    8
#define PARENT_LIVE_SEC 4

int main(void) {
    printf("[zombie_factory] PID=%d PPID=%d: forking %d children...\n",
           getpid(), getppid(), NUM_CHILDREN);
    fflush(stdout);

    for (int i = 0; i < NUM_CHILDREN; i++) {
        pid_t c = fork();
        if (c < 0) { perror("fork"); exit(1); }
        if (c == 0) {
            printf("[child-%d] PID=%d exiting\n", i+1, getpid());
            fflush(stdout);
            exit(0);
        }
        printf("[zombie_factory] forked child-%d PID=%d\n", i+1, c);
        fflush(stdout);
    }

    printf("[zombie_factory] sleeping %ds then exit (no wait)\n", PARENT_LIVE_SEC);
    fflush(stdout);
    sleep(PARENT_LIVE_SEC);
    printf("[zombie_factory] exiting → orphans go to PID 1\n");
    fflush(stdout);
    return 0;
}

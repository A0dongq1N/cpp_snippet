#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <signal.h>
#include <errno.h>
#include <string.h>

/**
 * 将当前进程转换为守护进程
 * 返回值：成功返回0，失败返回-1
 */
int daemonize() {
    pid_t pid;
    
    // 第一次 fork - 创建子进程
    pid = fork();
    if (pid < 0) {
        fprintf(stderr, "第一次 fork 失败: %s\n", strerror(errno));
        return -1;
    } else if (pid > 0) {
        // 父进程退出
        printf("父进程退出，子进程 PID: %d\n", pid);
        exit(0);
    }
    
    // 子进程继续执行
    printf("子进程开始守护进程化...\n");
    
    // 创建新的会话，脱离控制终端
    if (setsid() < 0) {
        fprintf(stderr, "setsid 失败: %s\n", strerror(errno));
        return -1;
    }
    
    // 忽略 SIGHUP 信号，防止会话领导者死亡时影响守护进程
    signal(SIGHUP, SIG_IGN);
    
    // 第二次 fork - 确保进程不是会话领导者
    pid = fork();
    if (pid < 0) {
        fprintf(stderr, "第二次 fork 失败: %s\n", strerror(errno));
        return -1;
    } else if (pid > 0) {
        // 第一个子进程退出
        exit(0);
    }
    
    // 最终的守护进程继续执行
    
    // 设置文件权限掩码为0，获得完全的权限控制
    umask(0);
    
    // 改变工作目录到根目录，避免占用可卸载的文件系统
    if (chdir("/") < 0) {
        fprintf(stderr, "chdir 失败: %s\n", strerror(errno));
        return -1;
    }
    
    // 重定向标准输入、输出、错误到 /dev/null
    int fd = open("/dev/null", O_RDWR);
    if (fd < 0) {
        fprintf(stderr, "打开 /dev/null 失败: %s\n", strerror(errno));
        return -1;
    }
    
    if (dup2(fd, STDIN_FILENO) < 0) {
        fprintf(stderr, "重定向 stdin 失败: %s\n", strerror(errno));
        close(fd);
        return -1;
    }
    
    if (dup2(fd, STDOUT_FILENO) < 0) {
        fprintf(stderr, "重定向 stdout 失败: %s\n", strerror(errno));
        close(fd);
        return -1;
    }
    
    if (dup2(fd, STDERR_FILENO) < 0) {
        // 注意：这里的错误信息可能无法显示，因为 stderr 正在被重定向
        close(fd);
        return -1;
    }
    
    // 关闭打开的文件描述符
    if (fd > STDERR_FILENO) {
        close(fd);
    }
    
    return 0;
}

/**
 * 信号处理函数
 */
void signal_handler(int sig) {
    switch (sig) {
        case SIGTERM:
            // 优雅退出
            exit(0);
            break;
        case SIGUSR1:
            // 用户自定义信号，可以用于重新加载配置等
            break;
        default:
            break;
    }
}

/**
 * 设置信号处理
 */
void setup_signals() {
    signal(SIGTERM, signal_handler);  // 终止信号
    signal(SIGUSR1, signal_handler);  // 用户信号1
    signal(SIGPIPE, SIG_IGN);         // 忽略管道破裂信号
}

int main() {
    printf("开始创建守护进程...\n");
    
    if (daemonize() < 0) {
        fprintf(stderr, "守护进程创建失败\n");
        return -1;
    }
    
    // 守护进程成功创建后，设置信号处理
    setup_signals();
    
    // 守护进程的主要工作循环
    // 注意：此时标准输出已经重定向到 /dev/null，printf 不会显示
    while (1) {
        // 这里可以添加守护进程的实际工作
        // 例如：
        // - 监控系统状态
        // - 处理网络请求
        // - 执行定时任务
        // - 写入日志文件等
        
        sleep(30);  // 每30秒执行一次循环
    }
    
    return 0;
}
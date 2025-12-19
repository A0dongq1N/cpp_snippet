# waitpid 相关知识点整理

本文档整理了关于 `waitpid_example.cc` 中涉及的 `waitpid` 函数及其状态解析宏的相关问答。

## 1. waitpid 函数参数详解

函数原型：
```cpp
pid_t waitpid(pid_t pid, int *status, int options);
```

### 参数 1：`pid` (等待的目标)
*   **`pid > 0`**: 等待进程 ID 等于该值的**特定子进程**。
*   **`pid == -1`**: 等待**任意**一个子进程（行为等同于 `wait`）。
*   **`pid == 0`**: 等待与当前进程处于**同一个进程组**的任何子进程。
*   **`pid < -1`**: 等待进程组 ID 等于 `|pid|` 的任何子进程。

### 参数 2：`status` (状态出口)
*   是一个输出型参数（指针）。
*   系统会将子进程的退出信息“打包”存入该整数中。
*   需要配合 `WIFEXITED` 等宏进行解析，不能直接当做退出码使用。

### 参数 3：`options` (控制选项)
*   **`0`**: 默认阻塞模式，父进程会一直死等子进程结束。
*   **`WNOHANG`**: 非阻塞模式，如果子进程没结束则立即返回 0。
*   **`WUNTRACED`**: 也会关注被暂停（Stopped）的子进程。

---

## 2. WIFEXITED 宏详解

### 全称与记忆
*   **全称**: **W**ait **IF** **EXITED**
*   **记忆逻辑**: **W**(Wait 家族) + **IF**(判断条件) + **EXITED**(正常退出)。
*   **含义**: “在 Wait 之后判断，如果进程是正常退出的”。

### 作用
检查子进程是否是“正常退出”的（即通过 `exit()`、`_exit()` 或 `main` 函数 `return` 结束）。
*   **返回真**: 子进程正常退出。
*   **返回假**: 子进程被信号杀掉（如 `kill -9`）等异常情况。

---

## 3. 常用宏组合使用示例

解析子进程状态的标准流程：

```cpp
int status;
waitpid(-1, &status, 0);

if (WIFEXITED(status)) {
    // 正常退出：获取退出码
    int exit_code = WEXITSTATUS(status);
    printf("正常退出，退出码: %d\n", exit_code);
} else if (WIFSIGNALED(status)) {
    // 信号终止：获取终止信号
    int sig = WTERMSIG(status);
    printf("被信号杀掉，信号编号: %d\n", sig);
}
```

### 记忆口诀
`W` 开头是 `wait` 家族，`WIF` 开头问“是不是”，后缀描述“啥情况”。


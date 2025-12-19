# wait vs waitpid 详解

本文档整理了关于 `wait` 和 `waitpid` 函数的区别及使用场景。

## 1. 核心区别对比

| 特性 | `wait(&status)` | `waitpid(pid, &status, options)` |
| :--- | :--- | :--- |
| **等待目标** | 任意子进程 | 可指定 PID (特定子进程) 或 -1 (任意子进程) |
| **阻塞模式** | 总是阻塞（父进程会死等子进程结束） | 可选非阻塞模式 (通过 `WNOHANG` 选项) |
| **功能范围** | 仅监控子进程终止 | 可监控终止、暂停 (Stopped) 和恢复 (Continued) |
| **灵活性** | 弱，适用于简单的收尾工作 | 强，适用于复杂的进程管理 |

## 2. 深入理解

### 等待对象 (pid 参数)
*   `wait(&status)` 相当于 `waitpid(-1, &status, 0)`。
*   `waitpid` 可以通过第一个参数精准控制：
    *   `pid > 0`: 等待特定 PID。
    *   `pid == -1`: 等待任意子进程（同 `wait`）。

### 非阻塞等待 (WNOHANG)
这是 `waitpid` 最重要的特性之一。
```c
// wait 会一直停在这里
wait(&status); 

// waitpid 设置 WNOHANG 后，如果孩子没死，立即返回 0
if (waitpid(-1, &status, WNOHANG) == 0) {
    // 子进程还在跑，父进程可以先干别的
}
```

### 功能等价性
在 Linux 系统底层，`wait` 函数通常是基于 `waitpid` 实现的一个简化版接口。

## 3. 使用建议

*   **使用 `wait` 的场景**：父进程只有一个子进程，或者父进程不需要做其他事情，只需要静静等待所有子进程结束。
*   **使用 `waitpid` 的场景**：
    1.  需要等待特定的子进程。
    2.  需要实现非阻塞等待（轮询检查子进程状态）。
    3.  需要处理作业控制（如监控进程的暂停和继续）。

---

## 4. 示例代码参考

### wait 示例 (`wait/wait_example.c`)
```c
int status;
wait(&status); // 阻塞直到子进程结束
```

### waitpid 示例 (`waitpid/waitpid_example.cc`)
```cpp
int status;
waitpid(-1, &status, 0); // 功能上等同于 wait(&status)
```


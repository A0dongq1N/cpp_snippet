# 僵尸进程复现 Demo

## 文件说明

| 文件 | 职责 |
|------|------|
| `bad_init.c` | Scene 1 PID 1：fork zombie_factory + monitor.sh，永不 waitpid() |
| `zombie_factory.c` | 制造孤儿：fork 8 子进程（不 wait），自身 4s 后退出 |
| `start.sh` | Scene 2 入口：启动 zombie_factory 后 exec monitor.sh |
| `monitor.sh` | 观测窗口：每 3 秒打印进程树 + zombie 计数 |
| `Dockerfile` | Scene 1：`/app/bad_init` 作为 PID 1 |
| `Dockerfile.tini` | Scene 2：**`/usr/bin/tini`** 作为 PID 1 |
| `run_demo.sh` | 一键 Docker 运行 |

## 快速运行

```bash
cd zombie_demo
bash run_demo.sh
```

## 进程架构

### Scene 1（bad_init 作 PID 1 → 僵尸堆积）
```
bad_init (PID 1, 不调用 waitpid)
  ├─ zombie_factory → fork 8 子进程后退出
  │    └─ child-1..8 (已退出，变成 bad_init 的孤儿僵尸)
  └─ monitor.sh (打印 zombie count > 0)
```

### Scene 2（tini 作 PID 1 → 自动 reap）
```
tini (PID 1, 循环 waitpid(-1))
  └─ start.sh → exec monitor.sh
       └─ zombie_factory (后台) → fork 8 子进程后退出
            └─ child-1..8 → 孤儿归 tini → 被 tini reap
                             monitor.sh 打印 zombie count = 0
```

## 核心对比

| | Scene 1 (bad_init) | Scene 2 (tini) |
|--|--|--|
| PID 1 | `/app/bad_init` | `/usr/bin/tini` |
| reap 逻辑 | 无 | `waitpid(-1, WNOHANG)` 循环 |
| zombie count | 9（持续堆积） | 0 |

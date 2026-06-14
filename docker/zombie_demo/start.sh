#!/usr/bin/env bash
# start.sh — Scene 2 的 tini 子进程入口
#
# 由 tini 启动，负责 fork zombie_factory 和 monitor.sh
# zombie_factory 退出后，孤儿被 tini（PID 1）自动 reap
echo "[start.sh] PID=$$, PPID=$PPID"
/app/zombie_factory &
exec /app/monitor.sh

#!/usr/bin/env bash
# monitor.sh — 每 3 秒打印进程树 + zombie 计数
# 由 bad_init 或 start.sh(exec) 启动

echo "[monitor.sh] PID=$$, PPID=$PPID — zombie watcher started"

tick=0
while true; do
    sleep 3
    tick=$((tick + 3))
    echo ""
    echo "===== $(date '+%H:%M:%S') [t=${tick}s] ====="
    echo "--- ps --forest ---"
    ps -ef --forest | head -25
    echo ""
    zombie_count=$(ps aux | awk '$8=="Z"{c++} END{print c+0}')
    echo ">>> ZOMBIE COUNT: ${zombie_count} <<<"
    if [ "$zombie_count" -gt 0 ]; then
        echo "--- zombie detail ---"
        ps aux | awk 'NR==1 || $8=="Z"'
    fi
done

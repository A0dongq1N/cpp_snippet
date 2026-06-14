#!/usr/bin/env bash
# run_demo.sh — 一键构建并演示
# 用法：cd zombie_demo && bash run_demo.sh
set -euo pipefail

DEMO_DURATION=20

echo "================================================================"
echo " Scene 1: bad_init 作为 PID 1（预期：zombie count > 0 堆积）"
echo "================================================================"
docker build -t zombie-demo:bad -f Dockerfile . -q
echo "[run_demo] 启动 Scene 1（${DEMO_DURATION}s）..."
docker run --rm --name zombie-bad --init=false zombie-demo:bad &
BAD_PID=$!
sleep $DEMO_DURATION
docker stop zombie-bad 2>/dev/null || true
wait $BAD_PID 2>/dev/null || true

echo ""
echo "================================================================"
echo " Scene 2: tini 作为 PID 1（预期：zombie count = 0）"
echo "================================================================"
docker build -t zombie-demo:tini -f Dockerfile.tini . -q
echo "[run_demo] 启动 Scene 2（${DEMO_DURATION}s）..."
docker run --rm --name zombie-tini --init=false zombie-demo:tini &
TINI_PID=$!
sleep $DEMO_DURATION
docker stop zombie-tini 2>/dev/null || true
wait $TINI_PID 2>/dev/null || true

echo ""
echo "=== Demo complete ==="

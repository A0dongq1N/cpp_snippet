#!/bin/bash

# 静态分析工具演示脚本
# 用于检测 size_t 下溢问题

echo "================================================"
echo "静态分析工具检测 size_t 下溢问题演示"
echo "================================================"
echo ""

TARGET_FILE="for/for_max_value.cc"
TEST_FILE="for/detect_underflow.cc"

# 1. GCC 编译器警告
echo "1. GCC 编译器警告 (-Wall -Wextra -Wconversion)"
echo "------------------------------------------------"
g++ -Wall -Wextra -Wconversion -Wsign-compare -Wsign-conversion -Werror "$TARGET_FILE" -o /tmp/test_gcc 2>&1
if [ $? -eq 0 ]; then
    echo "✓ 编译成功（但编译器未检测到下溢问题）"
else
    echo "✗ 编译失败"
fi
echo ""

# 2. Clang 编译器警告
echo "2. Clang 编译器警告"
echo "------------------------------------------------"
clang++ -Wall -Wextra -Wconversion -Wsign-compare -Wsign-conversion -Werror "$TARGET_FILE" -o /tmp/test_clang 2>&1
if [ $? -eq 0 ]; then
    echo "✓ 编译成功（但编译器未检测到下溢问题）"
else
    echo "✗ 编译失败"
fi
echo ""

# 3. Clang-Tidy
echo "3. Clang-Tidy 静态分析"
echo "------------------------------------------------"
clang-tidy "$TARGET_FILE" -checks='-*,bugprone-*,cert-*,clang-analyzer-*,misc-*' -- -std=c++11 2>&1 | grep -v "non-user code" | head -20
if [ ${PIPESTATUS[0]} -eq 0 ]; then
    echo "✓ Clang-Tidy 分析完成"
else
    echo "✗ Clang-Tidy 分析失败"
fi
echo ""

# 4. Clang Static Analyzer
echo "4. Clang Static Analyzer"
echo "------------------------------------------------"
clang++ --analyze -Xanalyzer -analyzer-output=text "$TARGET_FILE" 2>&1
if [ $? -eq 0 ]; then
    echo "✓ 静态分析完成（未发现问题）"
else
    echo "✗ 静态分析发现问题"
fi
echo ""


# 5. Cppcheck (如果安装了)
echo "6. Cppcheck 静态分析"
echo "------------------------------------------------"
if command -v cppcheck &> /dev/null; then
    cppcheck --enable=all --inconclusive --suppress=missingIncludeSystem "$TARGET_FILE" 2>&1
    echo "✓ Cppcheck 分析完成"
else
    echo "✗ Cppcheck 未安装"
    echo "  安装命令: sudo apt-get install cppcheck"
fi
echo ""

# 总结
echo "================================================"
echo "总结"
echo "================================================"
echo "❌ 问题：编译器和大多数静态分析工具很难检测到"
echo "   size_t 下溢问题，因为："
echo "   1. 无符号整数下溢是定义良好的行为（不是 UB）"
echo "   2. 编译时无法确定运行时的变量值"
echo "   3. 需要流程敏感的数据流分析"
echo ""
echo "✅ 建议的解决方案："
echo "   1. 代码审查 - 重点关注无符号类型的减法操作"
echo "   2. 编码规范 - 在减法前检查大小关系"
echo "   3. 使用有符号类型（int64_t）而不是 size_t"
echo "   4. 使用更高级的商业工具（Coverity, PVS-Studio）"
echo "   5. 添加运行时断言检查"
echo "================================================"

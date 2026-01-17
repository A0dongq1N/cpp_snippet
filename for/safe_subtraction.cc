#include <iostream>
#include <cstddef>
#include <cassert>
#include <stdexcept>

// ============================================================================
// 安全的无符号减法实现
// ============================================================================

/**
 * 方案1: 返回布尔值表示是否成功
 */
bool safe_subtract_v1(size_t a, size_t b, size_t& result) {
    if (a < b) {
        return false;  // 下溢，返回失败
    }
    result = a - b;
    return true;
}

/**
 * 方案2: 抛出异常
 */
size_t safe_subtract_v2(size_t a, size_t b) {
    if (a < b) {
        throw std::underflow_error("size_t subtraction would underflow");
    }
    return a - b;
}

/**
 * 方案3: 返回 0（饱和运算）
 */
size_t safe_subtract_v3(size_t a, size_t b) {
    return (a > b) ? (a - b) : 0;
}

/**
 * 方案4: 使用断言（调试版本）
 */
size_t safe_subtract_v4(size_t a, size_t b) {
    assert(a >= b && "size_t subtraction would underflow");
    return a - b;
}

/**
 * 方案5: 编译时检查（C++11）
 */
template<size_t A, size_t B>
struct SafeSubtract {
    static_assert(A >= B, "Compile-time underflow detected");
    static constexpr size_t value = A - B;
};

// ============================================================================
// 实际使用示例
// ============================================================================

// 错误的循环
void bad_loop_example(size_t start, size_t end) {
    std::cout << "❌ 错误示例：" << std::endl;
    size_t count = end - start;  // 危险！如果 end < start 会下溢
    std::cout << "  循环次数: " << count << std::endl;
    
    // 这个循环可能执行天文数字次
    // for (size_t i = 0; i < count; ++i) { ... }
}

// 正确的循环 - 方法1：使用有符号类型
void good_loop_v1(int64_t start, int64_t end) {
    std::cout << "✅ 正确示例1 (有符号类型)：" << std::endl;
    int64_t count = end - start;
    std::cout << "  循环次数: " << count << std::endl;
    
    if (count > 0) {
        for (int64_t i = 0; i < count; ++i) {
            // 安全的循环
        }
        std::cout << "  循环执行完成" << std::endl;
    } else {
        std::cout << "  跳过循环 (count <= 0)" << std::endl;
    }
}

// 正确的循环 - 方法2：先检查
void good_loop_v2(size_t start, size_t end) {
    std::cout << "✅ 正确示例2 (先检查)：" << std::endl;
    
    if (end <= start) {
        std::cout << "  跳过循环 (end <= start)" << std::endl;
        return;
    }
    
    size_t count = end - start;
    std::cout << "  循环次数: " << count << std::endl;
    
    for (size_t i = 0; i < count; ++i) {
        // 安全的循环
    }
    std::cout << "  循环执行完成" << std::endl;
}

// 正确的循环 - 方法3：使用安全函数
void good_loop_v3(size_t start, size_t end) {
    std::cout << "✅ 正确示例3 (安全函数)：" << std::endl;
    
    size_t count;
    if (!safe_subtract_v1(end, start, count)) {
        std::cout << "  跳过循环 (检测到下溢)" << std::endl;
        return;
    }
    
    std::cout << "  循环次数: " << count << std::endl;
    for (size_t i = 0; i < count; ++i) {
        // 安全的循环
    }
    std::cout << "  循环执行完成" << std::endl;
}

// 正确的循环 - 方法4：迭代器风格
void good_loop_v4(size_t start, size_t end) {
    std::cout << "✅ 正确示例4 (迭代器风格)：" << std::endl;
    
    // 不计算差值，直接迭代
    size_t count = 0;
    for (size_t i = start; i < end; ++i) {
        count++;
        // 处理索引 i
    }
    std::cout << "  循环执行了 " << count << " 次" << std::endl;
}

// ============================================================================
// 代码审查检查清单
// ============================================================================
void print_code_review_checklist() {
    std::cout << "\n" << std::string(70, '=') << std::endl;
    std::cout << "代码审查检查清单 - 无符号整数减法" << std::endl;
    std::cout << std::string(70, '=') << std::endl;
    
    std::cout << R"(
📋 在代码审查时需要特别关注：

1. ⚠️ 无符号类型的减法操作
   - size_t result = a - b;  // 如果 a < b 会下溢
   - unsigned int diff = x - y;
   
2. ⚠️ 循环边界使用无符号减法
   - for (size_t i = 0; i < (end - start); ++i)
   
3. ⚠️ 数组索引计算
   - arr[index - offset]  // 如果 index < offset 会下溢
   
4. ⚠️ 函数参数（特别是 size_t）
   void func(size_t a, size_t b) {
       size_t diff = a - b;  // 危险！
   }

✅ 安全的替代方案：

1. 使用有符号类型
   int64_t result = static_cast<int64_t>(a) - static_cast<int64_t>(b);
   
2. 先检查大小
   if (a >= b) {
       size_t result = a - b;
   }
   
3. 使用条件表达式
   size_t result = (a > b) ? (a - b) : 0;
   
4. 使用安全函数
   size_t result;
   if (safe_subtract(a, b, result)) {
       // 使用 result
   }
   
5. 添加运行时断言
   assert(a >= b && "potential underflow");
   size_t result = a - b;

)" << std::endl;
}

// ============================================================================
// 主程序
// ============================================================================
int main() {
    std::cout << std::string(70, '=') << std::endl;
    std::cout << "安全的 size_t 减法和循环示例" << std::endl;
    std::cout << std::string(70, '=') << std::endl;
    std::cout << std::endl;
    
    // 测试用例
    size_t start = 10;
    size_t end = 5;  // end < start，会导致下溢
    
    std::cout << "测试参数: start = " << start << ", end = " << end << std::endl;
    std::cout << std::string(70, '-') << std::endl;
    std::cout << std::endl;
    
    // 错误示例
    bad_loop_example(start, end);
    std::cout << std::endl;
    
    // 正确示例
    good_loop_v1(static_cast<int64_t>(start), static_cast<int64_t>(end));
    std::cout << std::endl;
    
    good_loop_v2(start, end);
    std::cout << std::endl;
    
    good_loop_v3(start, end);
    std::cout << std::endl;
    
    good_loop_v4(start, end);
    std::cout << std::endl;
    
    // 测试安全函数
    std::cout << std::string(70, '-') << std::endl;
    std::cout << "测试安全减法函数：" << std::endl;
    std::cout << std::string(70, '-') << std::endl;
    
    size_t result;
    if (safe_subtract_v1(10, 5, result)) {
        std::cout << "✓ 10 - 5 = " << result << std::endl;
    }
    
    if (!safe_subtract_v1(5, 10, result)) {
        std::cout << "✓ 5 - 10 检测到下溢，返回失败" << std::endl;
    }
    
    std::cout << "✓ 使用饱和运算: 5 - 10 = " << safe_subtract_v3(5, 10) << std::endl;
    
    try {
        safe_subtract_v2(5, 10);
    } catch (const std::underflow_error& e) {
        std::cout << "✓ 捕获异常: " << e.what() << std::endl;
    }
    
    // 编译时检查（会通过）
    constexpr size_t compile_time_result = SafeSubtract<10, 5>::value;
    std::cout << "✓ 编译时检查: 10 - 5 = " << compile_time_result << std::endl;
    
    // 下面这行会导致编译错误（取消注释试试）
    // constexpr size_t error = SafeSubtract<5, 10>::value;
    
    // 打印检查清单
    print_code_review_checklist();
    
    return 0;
}

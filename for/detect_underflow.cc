#include <iostream>
#include <cstddef>

// 更明显的错误示例，编译器可能会警告
void obvious_error() {
    size_t a = 5;
    size_t b = 10;
    
    // 直接在常量表达式中，编译器可能检测到
    size_t result = 5 - 10;  // 编译时常量
    std::cout << "5 - 10 = " << result << std::endl;
}

// 运行时才能确定的错误
void runtime_error(size_t a, size_t b) {
    size_t result = a - b;
    std::cout << a << " - " << b << " = " << result << std::endl;
    
    // 危险的循环
    if (result < 1000000) {  // 添加保护，但仍然危险
        for (size_t i = 0; i < result; ++i) {
            // 做一些操作
        }
    } else {
        std::cout << "警告：循环次数过大 " << result << std::endl;
    }
}

int main(int argc, char* argv[]) {
    std::cout << "=== 编译时可检测的错误 ===" << std::endl;
    obvious_error();
    
    std::cout << "\n=== 运行时错误 ===" << std::endl;
    
    // 从用户输入或参数获取值
    size_t a = (argc > 1) ? std::atoi(argv[1]) : 5;
    size_t b = (argc > 2) ? std::atoi(argv[2]) : 10;
    
    runtime_error(a, b);
    
    return 0;
}

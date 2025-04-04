#include <vector>
#include <iostream>
#include <algorithm>
#include <chrono>   
#include <iomanip>  

// 使用迭代器遍历
void print_vector(const std::vector<int>& vec) {
    for (auto it = vec.begin(); it != vec.end(); ++it) {
        // 为了公平比较，我们不实际打印，而是做一些操作
        volatile int temp = *it;  // 防止编译器优化掉
    }
}

// 使用范围for循环遍历
void print_vector_range_based(const std::vector<int>& vec) {
    for (int i : vec) {
        volatile int temp = i;
    }
}

// 使用std::for_each遍历
void print_vector_algorithm(const std::vector<int>& vec) {
    std::for_each(vec.begin(), vec.end(), [](int i) {
        volatile int temp = i;
    });
}

// 测试函数，返回执行时间（微秒）
template<typename Func>
double measure_time(Func func, const std::vector<int>& vec, int iterations) {
    auto start = std::chrono::high_resolution_clock::now();
    
    for(int i = 0; i < iterations; ++i) {
        func(vec);
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    return duration.count() / static_cast<double>(iterations);
}

int main() {
    // 创建一个较大的vector用于测试
    const int SIZE = 1000000;  // 100万个元素
    const int ITERATIONS = 1000;  // 每种方法测试1000次取平均值
    
    std::vector<int> numbers(SIZE);
    // 初始化vector
    for(int i = 0; i < SIZE; ++i) {
        numbers[i] = i;
    }

    std::cout << "测试vector大小: " << SIZE << " 元素" << std::endl;
    std::cout << "每种方法重复测试: " << ITERATIONS << " 次" << std::endl;
    std::cout << std::fixed << std::setprecision(3) << std::endl;

    // 测试迭代器方式
    double iterator_time = measure_time(print_vector, numbers, ITERATIONS);
    std::cout << "迭代器遍历平均时间: " << iterator_time << " 微秒" << std::endl;

    // 测试范围for循环方式
    double range_based_time = measure_time(print_vector_range_based, numbers, ITERATIONS);
    std::cout << "范围for循环遍历平均时间: " << range_based_time << " 微秒" << std::endl;

    // 测试std::for_each方式
    double algorithm_time = measure_time(print_vector_algorithm, numbers, ITERATIONS);
    std::cout << "std::for_each遍历平均时间: " << algorithm_time << " 微秒" << std::endl;

    // 输出性能比较
    std::cout << "\n性能比较（以最快方法为基准）：" << std::endl;
    double min_time = std::min({iterator_time, range_based_time, algorithm_time});
    
    std::cout << "迭代器方式: " << (iterator_time / min_time) << "x" << std::endl;
    std::cout << "范围for循环: " << (range_based_time / min_time) << "x" << std::endl;
    std::cout << "std::for_each: " << (algorithm_time / min_time) << "x" << std::endl;

    return 0;
}

/*
最佳实践建议
"在实际开发中，我建议：
优先使用范围for循环，因为：
代码最简洁清晰
不容易出错
编译器优化良好
在以下情况使用迭代器：
需要访问元素位置
需要特殊的迭代控制
需要修改容器结构
在以下情况使用 std::for_each：
需要并行执行
与其他算法库配合使用
需要函数式编程风格"
补充说明
"需要注意的是：
真实性能瓶颈通常不在遍历方式上，而在于：
内存访问模式
缓存利用
具体的数据处理逻辑
代码可维护性和清晰度往往比微小的性能差异更重要
如果确实需要极致性能，应该：
进行实际场景的性能测试
考虑使用并行算法
关注数据布局和内存访问模式"
*/
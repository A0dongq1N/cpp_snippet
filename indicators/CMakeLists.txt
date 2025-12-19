# 指定 CMake 最小版本要求
cmake_minimum_required(VERSION 3.14)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# 定义项目名称
project(indicators_demo)

# 设置 C++ 标准为 C++11
set(CMAKE_CXX_STANDARD 11)
# 强制要求编译器必须支持选定的标准
set(CMAKE_CXX_STANDARD_REQUIRED True)

# 引入 FetchContent 模块，用于从网络下载第三方库
include(FetchContent)

# 声明远程库的信息（GitHub 地址和版本号）
FetchContent_Declare(
  indicators
  GIT_REPOSITORY https://github.com/p-ranav/indicators.git
  GIT_TAG        v2.3
)

# 下载并加载 indicators 库
FetchContent_MakeAvailable(indicators)

# 生成可执行程序
add_executable(indicators_demo main.cpp)

# 将 indicators 库链接到可执行程序
target_link_libraries(indicators_demo PRIVATE indicators)


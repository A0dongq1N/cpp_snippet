cmake_minimum_required(VERSION 3.10)
project(crypto)

set(CMAKE_C_STANDARD 99) # 或者 11, 17 等
set(CMAKE_C_STANDARD_REQUIRED ON)

# 2. 正确设置编译标志 (推荐使用 add_compile_options)
add_compile_options(-Wall -Wextra -O2)

add_executable(af_alg af_alg.c)
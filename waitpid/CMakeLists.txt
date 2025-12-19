cmake_minimum_required(VERSION 3.10)
project(waitpid C CXX)

include_directories(/usr/include)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -O2")

add_executable(waitpid_example waitpid_example.cc)
add_executable(wuntraced_wcontinued_demo wuntraced_wcontinued_demo.c)
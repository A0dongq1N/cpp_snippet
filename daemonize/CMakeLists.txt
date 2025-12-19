cmake_minimum_required(VERSION 3.10)
project(daemonize C CXX)

include_directories(/usr/include)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -O2")

add_executable(daemonize_demo daemonize_demo.c)
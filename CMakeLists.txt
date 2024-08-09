# 设置最低要求的CMake版本
cmake_minimum_required(VERSION 3.10)

# 设置项目名称
project(MemoryManager)

# 设置C标准
set(CMAKE_C_STANDARD 99)

# 添加头文件目录
include_directories(${CMAKE_SOURCE_DIR}/trace_memcall)

# 搜索指定目录下的所有源文件
file(GLOB SOURCES "main.c" "trace_memcall/*.c")

# 添加可执行文件
add_executable(${PROJECT_NAME} ${SOURCES})

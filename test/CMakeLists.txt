cmake_minimum_required(VERSION 3.10)
project(snake_benchmark)

# 使用 C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# 查找 SFML 库
find_package(SFML 2.5 COMPONENTS graphics window system audio REQUIRED)

# 查找 google benchmark 库
find_package(benchmark REQUIRED)

# 添加源文件
file(GLOB_RECURSE SOURCES "src/*.cpp")
add_executable(snake_benchmark ${SOURCES})

# 链接 SFML 和 google benchmark 库
target_link_libraries(snake_benchmark sfml-graphics sfml-window sfml-system sfml-audio benchmark::benchmark)

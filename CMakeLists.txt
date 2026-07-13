cmake_minimum_required(VERSION 3.16)

project(MiniSearchEngine)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Compiler warnings
if(MSVC)
    add_compile_options(/W4)
else()
    add_compile_options(-Wall -Wextra -O2)
endif()

# ===========================
# Main Executable
# ===========================

add_executable(MiniSearchEngine
    src/main.cpp
    src/Trie.cpp
)

target_include_directories(MiniSearchEngine PRIVATE src)

# ===========================
# Benchmark Executable
# ===========================

add_executable(benchmark
    src/benchmark.cpp
    src/Trie.cpp
)

target_include_directories(benchmark PRIVATE src)

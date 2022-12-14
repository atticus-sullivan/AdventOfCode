cmake_minimum_required(VERSION 3.21)

project(AoC2022_Cpp)

set(CMAKE_CXX_STANDARD 20)

# so language servers like clangd know what the buildsystem does
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# # windows needs "exported" symbols for linking
# set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

# display many warnings during compilation
# they usually help you find some bugs
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang|AppleClang")
    add_compile_options(-Wall -Wextra -Wconversion -pedantic -Wfatal-errors -g)
elseif(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    add_compile_options(/W3 /WX)

    # so the dll files are alongside the executables
    # and the windows loader is happy (no more error 0xc0000135)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
endif()

add_executable(day01 day01.cpp)
add_executable(day02 day02.cpp)
add_executable(day03 day03.cpp)
add_executable(day04 day04.cpp)
add_executable(day05 day05.cpp)
add_executable(day06 day06.cpp)
add_executable(day07 day07.cpp)
add_executable(day08 day08.cpp)
add_executable(day09 day09.cpp)
add_executable(day10 day10.cpp)
add_executable(day11 day11.cpp)
add_executable(day12 day12.cpp)
add_executable(day13 day13.cpp)
add_executable(day14 day14.cpp)

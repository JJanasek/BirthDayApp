cmake_minimum_required(VERSION 3.5)
project(BirthDay)

set(CMAKE_C_STANDARD 99)

add_executable(BirthDay main.c)

target_compile_options(BirthDay PRIVATE -pedantic -Wall -Wextra -Werror -O2)

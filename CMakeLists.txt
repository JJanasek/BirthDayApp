cmake_minimum_required(VERSION 3.5)
project(BirthDay)

set(CMAKE_C_STANDARD 99)

add_executable(BirthDay main.c heap.c storage.c utils.c)

target_compile_options(BirthDay PRIVATE -pedantic -Wall -Wextra -Werror -O2)

find_package(PkgConfig REQUIRED)
pkg_check_modules(LIBNOTIFY REQUIRED libnotify)


target_include_directories(BirthDay PRIVATE ${LIBNOTIFY_INCLUDE_DIRS})
target_link_libraries(BirthDay PRIVATE ${LIBNOTIFY_LIBRARIES})

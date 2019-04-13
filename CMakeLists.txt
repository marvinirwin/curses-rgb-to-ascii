cmake_minimum_required(VERSION 3.13)
project(untitled C)
set(CMAKE_C_STANDARD 99)
find_package(curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIR})

SET(FLAG32 -m32)
SET(CMAKE_C_FLAGS "-m32")

add_executable(untitled main.c)

target_link_libraries(untitled ${CURSES_LIBRARIES} ${FLAGS32})



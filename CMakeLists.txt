cmake_minimum_required(VERSION 3.22)
project(my_x86_os C)

set(CMAKE_CXX_STANDARD 98)

add_executable(my_x86_os main.c tools/common.h tools/fileinfo.c tools/floppy.c)

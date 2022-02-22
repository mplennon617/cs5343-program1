cmake_minimum_required(VERSION 3.16)
project(program1 C)

set(CMAKE_C_STANDARD 99)

# Copy data files to build dir
configure_file("prog1_test.txt" "prog1_test.txt" COPYONLY)

add_executable(program1 main.c)

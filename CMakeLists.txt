cmake_minimum_required(VERSION 3.16)
project(ntutils C)

set(CMAKE_C_STANDARD 11)

add_library(ntutils
        include/nt/config.h
        include/nt/macros.h
        include/nt/panic.h

        source/panic.c
        )

target_include_directories(ntutils PUBLIC include)

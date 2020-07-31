cmake_minimum_required(VERSION 3.16)
project(ntutils C)

set(CMAKE_C_STANDARD 11)

add_library(ntutils
        include/nt/assert.h
        include/nt/circular.h
        include/nt/compare.h
        include/nt/config.h
        include/nt/macros.h
        include/nt/measurement.h
        include/nt/mem.h
        include/nt/panic.h
        include/nt/test.h

        source/circular.c
        source/panic.c
        source/measurement.c
        source/mem.c
        source/test.c)

target_include_directories(ntutils PUBLIC include)

target_compile_options(ntutils PRIVATE
        $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>,$<CXX_COMPILER_ID:GNU>>:
        -Wall -Werror>)

add_executable(ntutils_mem_tests
        tests/mem.c)

add_test(NAME ntutils_mem_tests COMMAND ntutils_str_tests)

target_link_libraries(ntutils_mem_tests ntutils)

add_executable(ntutils_circular_tests
        tests/circular.c)

add_test(NAME ntutils_circular_tests COMMAND ntutils_str_tests)

target_link_libraries(ntutils_circular_tests ntutils)

cmake_minimum_required(VERSION 3.16)
project(ntutils C)

set(CMAKE_C_STANDARD 11)

add_library(ntutils
        include/nt/assert.h
        include/nt/circular.h
        include/nt/compare.h
        include/nt/config.h
        include/nt/json.h
        include/nt/list.h
        include/nt/macros.h
        include/nt/measurement.h
        include/nt/mem.h
        include/nt/panic.h
        include/nt/reader.h
        include/nt/stream.h
        include/nt/test.h
        include/nt/utf8.h

        source/circular.c
        source/json.c
        source/list.c
        source/measurement.c
        source/mem.c
        source/panic.c
        source/reader.c
        source/stream.c
        source/test.c
        source/utf8.c)

target_include_directories(ntutils PUBLIC include)

target_compile_options(ntutils PRIVATE -Wall -Wextra -Werror)

function(create_test name sources)
    add_executable(${name} ${sources})

    add_test(NAME ${name} COMMAND ${name})

    target_link_libraries(${name} ntutils)
endfunction()

create_test(ntutils_circular_tests tests/circular.c)
create_test(ntutils_json_tests tests/json.c)
create_test(ntutils_list_tests tests/list.c)
create_test(ntutils_mem_tests tests/mem.c)
create_test(ntutils_reader_tests tests/reader.c)
create_test(ntutils_utf8_tests tests/utf8.c)

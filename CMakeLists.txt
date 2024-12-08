cmake_minimum_required(VERSION 3.10)

project(Neptune-OS) # Fill in the project name.

# Set C compiler
set(CMAKE_COMPILER gcc)

# Set C++ compiler
set(CMAKE_CXX_COMPILER g++)

# set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -Wpedantic -Werror")
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -g -O0")
set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -O3")
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

set(CMAKE_C_STANDARD 11)

add_executable(${PROJECT_NAME}
    kernel.c
    lib/cJSON.c
    lib/seqft/common.c
    lib/seqft/evaluator.c
    lib/seqft/stack.c
    lib/seqft/tokenizer.c
    terminal.c
    boot.asm
)

# Libraries to be statically linked with executable.  
target_link_libraries(${PROJECT_NAME}
    math
)

## Additional library search directories.
# target_link_directories(${PROJECT_NAME}
# )

## Include directories.
# target_include_directories(${PROJECT_NAME}
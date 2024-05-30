cmake_minimum_required(VERSION 3.5.1)
project(lab5 C)

include_directories(include)
set(HEADERS include/bit_stream.h include/code.h include/constants.h include/debug_macros.h include/huffman.h include/queue.h include/tree.h)

set(SRC src/main.c src/bit_stream.c src/code.c src/huffman.c src/queue.c src/tree.c)
add_executable(${PROJECT_NAME} ${SRC} ${HEADERS})

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/../modules")
include(common_lab)

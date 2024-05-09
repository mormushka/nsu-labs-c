cmake_minimum_required(VERSION 3.5.1)
project(lab6-1 C)

include_directories(include)
set(HEADERS include/memory.h include/m_math.h include/rb_tree.h)
set(SRC src/main.c src/memory.c src/m_math.c src/rb_tree.c)

add_executable(${PROJECT_NAME} ${SRC} ${HEADERS})

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/../modules")
include(common_lab)
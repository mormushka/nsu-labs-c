cmake_minimum_required(VERSION 3.5.1)
project(lab9 C)

include_directories(include)
set(HEADERS include/list.h include/graph.h include/dijkstra.h include/output.h)
set(SRC src/main.c src/list.c src/graph.c src/dijkstra.c src/output.c)
add_executable(${PROJECT_NAME} ${SRC} ${HEADERS})

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/../modules")
include(common_lab)

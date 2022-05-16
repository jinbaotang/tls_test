cmake_minimum_required(VERSION 2.8)
project(tls_test)

MESSAGE(STATUS "TTTTTTTTTT PROJECT_NAME = ${PROJECT_NAME}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread -std=c++11")
set(ENV{TLS_RUNTIME_PATH} ${CMAKE_SOURCE_DIR})
MESSAGE(STATUS "TTTTTTTTTT RUNTIME_PATH = $ENV{TLS_RUNTIME_PATH}")



add_executable (${PROJECT_NAME}_client $ENV{TLS_RUNTIME_PATH}/tls_client.c)
target_link_libraries(${PROJECT_NAME}_client ssl dl crypto)


add_executable (${PROJECT_NAME}_server $ENV{TLS_RUNTIME_PATH}/tls_server.c)
target_link_libraries(${PROJECT_NAME}_server ssl dl crypto)

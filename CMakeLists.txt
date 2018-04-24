cmake_minimum_required(VERSION 3.9)
project(producer_consumer)

set(CMAKE_CXX_STANDARD 11)

add_executable(producer_consumer
        main.c
        buffer.h)

cmake_minimum_required(VERSION 3.10.0)

project(Semaphore_Cpp11)

add_library(Semaphore_Cpp11 INTERFACE)

target_include_directories(Semaphore_Cpp11 INTERFACE
    include
)

enable_testing()
add_subdirectory(tests)
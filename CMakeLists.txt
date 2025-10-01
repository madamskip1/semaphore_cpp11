cmake_minimum_required(VERSION 3.10.0)

project(CountingSemaphore_Cpp11)

add_library(CountingSemaphore_Cpp11 INTERFACE)

target_include_directories(CountingSemaphore_Cpp11 INTERFACE
    include
)

enable_testing()
add_subdirectory(tests)
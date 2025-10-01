cmake_minimum_required(VERSION 3.10.0)

project(Semaphore_Cpp11)

add_library(MA_Semaphore_Cpp11 INTERFACE)
add_library(MA::Semaphore_Cpp11 ALIAS MA_Semaphore_Cpp11)

target_include_directories(MA_Semaphore_Cpp11 INTERFACE
    include
)

enable_testing()
add_subdirectory(tests)
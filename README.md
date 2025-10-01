# Counting Semaphore C++11 

A small, header-only semaphore implementation for C++11 and later.
It provides a [**std::counting_semaphore-like**](https://en.cppreference.com/w/cpp/thread/counting_semaphore.html) interface.

## Requirements

- C++11 or higher
- No external dependencies


## Installation

### Single-header copy-paste

Simply copy-paste the [semaphore.hpp](https://github.com/madamskip1/semaphore_cpp11/blob/master/include/MA/semaphore.hpp) header file into your project.

or...

### FetchContent

```cmake
include(FetchContent)

FetchContent_Declare(
  MA_Sepahore_Cpp11
  GIT_REPOSITORY https://github.com/madamskip1/semaphore_cpp11.git
  GIT_TAG        v1.0.0
)

FetchContent_MakeAvailable(MA_Sepahore_Cpp11)

target_link_libraries(your_target PRIVATE MA::Semaphore_Cpp11)
```



## Usage

The API is designed to match [`std::counting_semaphore`](https://en.cppreference.com/w/cpp/thread/counting_semaphore.html) from C++20, so you can use it in the same way.

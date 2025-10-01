set(CMAKE_CXX_STANDARD 11)

include(FetchContent)
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG        v1.17.0
)
FetchContent_MakeAvailable(googletest)


add_executable(CountingSemaphore_Cxx11_tests)

target_sources(CountingSemaphore_Cxx11_tests
    PRIVATE
        tests.cpp
)

target_link_libraries(CountingSemaphore_Cxx11_tests
    PRIVATE
        gtest_main
        CountingSemaphore_Cxx11
)

include(GoogleTest)
gtest_discover_tests(CountingSemaphore_Cxx11_tests)
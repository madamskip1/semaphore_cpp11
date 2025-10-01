set(CMAKE_CXX_STANDARD 11)

include(FetchContent)
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG        v1.17.0
)
FetchContent_MakeAvailable(googletest)


add_executable(Semaphore_Cpp11_tests)

target_sources(Semaphore_Cpp11_tests
    PRIVATE
        tests.cpp
)

target_link_libraries(Semaphore_Cpp11_tests
    PRIVATE
        gtest_main
        Semaphore_Cpp11
)

include(GoogleTest)
gtest_discover_tests(Semaphore_Cpp11_tests)
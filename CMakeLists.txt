cmake_minimum_required(VERSION 3.10.0)

project(MA_Semaphore_Cpp11
    VERSION 1.0.0
    LANGUAGES CXX
)

option(MA_Semaphore_Cpp11_BUILD_TESTING "Build tests for MA MA_Semaphore_Cpp11" OFF)
option(MA_Semaphore_Cpp11_INSTALL "Enable installation of MA::Semaphore_Cpp11 targets and config files" OFF)


add_library(MA_Semaphore_Cpp11 INTERFACE)
add_library(MA::Semaphore_Cpp11 ALIAS MA_Semaphore_Cpp11)

target_include_directories(MA_Semaphore_Cpp11
    INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)


if (MA_Semaphore_Cpp11_INSTALL)
    install(TARGETS MA_Semaphore_Cpp11
        EXPORT MA_Semaphore_Cpp11Targets
        INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}
    )

    install(EXPORT MA_Semaphore_Cpp11Targets
        FILE MA_Semaphore_Cpp11Targets.cmake
        NAMESPACE MA::
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/MA/semaphore
    )

    install(DIRECTORY include/MA
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    )

    export(EXPORT MA_Semaphore_Cpp11Targets
        FILE MA_Semaphore_Cpp11Targets.cmake
        NAMESPACE MA::
    )

    include(CMakePackageConfigHelpers)

    set(MA_Semaphore_Cpp11_version_file_name "${PROJECT_NAME}-Version.cmake")
    write_basic_package_version_file(
        "${CMAKE_CURRENT_BINARY_DIR}/${MA_Semaphore_Cpp11_version_file_name}"
        VERSION ${PROJECT_VERSION}
        COMPATIBILITY SameMajorVersion
    )

    install(FILES
        "${CMAKE_CURRENT_BINARY_DIR}/${MA_Semaphore_Cpp11_version_file_name}"
        DESTINATION lib/cmake/MA/semaphore
    )
endif ()


if (MA_Semaphore_Cpp11_BUILD_TESTING)
    enable_testing()
    add_subdirectory(tests)
endif()
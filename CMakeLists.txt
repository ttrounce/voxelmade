cmake_minimum_required(VERSION 3.17)

project(voxelmade)
set(PROJECT_LIB voxelmadelib)

# Sources and Includes

file(GLOB SOURCES
    src/*.cpp
    src/*.h
)
file(GLOB TEST_SOURCES tests/*.cpp)
list(REMOVE_ITEM SOURCES src/main.cpp)

# External Dependencies

add_subdirectory(dependencies/glfw)
add_subdirectory(dependencies/glad)

# Making Libraries and Executable

add_library(${PROJECT_LIB} ${SOURCES})
add_executable(${PROJECT_NAME} src/main.cpp)

target_link_libraries(${PROJECT_LIB} glfw)
target_link_libraries(${PROJECT_LIB} glad)
target_link_libraries(${PROJECT_NAME} ${PROJECT_LIB})

# Tests

option(BUILD_TESTS "Build tests" OFF)

if(BUILD_TESTS)
    # Google Test Dependency
    add_subdirectory(dependencies/googletest)

    enable_testing()
    include(CTest)

    add_executable(runUnitTests ${TEST_SOURCES})
    target_link_libraries(runUnitTests gtest gtest_main)
    target_link_libraries(runUnitTests ${PROJECT_LIB})

    add_test(UnitTests runUnitTests)
endif()
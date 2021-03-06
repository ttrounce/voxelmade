cmake_minimum_required(VERSION 3.17)

project(voxelmade)
set(PROJECT_LIB voxelmade_lib)
set(PROJECT_TESTS voxelmade_tests)

if(BUILD_TYPE EQUAL 0)
    # Debug
    message("[BUILD TYPE] Debug")
    set(CMAKE_BUILD_TYPE "Debug")
    set(CMAKE_CXX_FLAGS "-std=gnu++14 -Werror -Wall -g")
    set(BUILD_TESTS "ON")
elseif(BUILD_TYPE EQUAL 1)
    # Release
    message("[BUILD TYPE] Release (With Tests)")
    set(CMAKE_BUILD_TYPE "Release")
    set(CMAKE_CXX_FLAGS "-std=gnu++14 -Werror -Wall -O3")
    set(BUILD_TESTS "ON")
elseif(BUILD_TYPE EQUAL 2)
    # Release (No Tests)
    message("[BUILD TYPE] Release (No Tests)")
    set(CMAKE_BUILD_TYPE "Release")
    set(CMAKE_CXX_FLAGS "-std=gnu++14 -Werror -Wall -O3")
endif()

# Sources and Includes

file(GLOB_RECURSE SOURCES
    src/*.cpp
    src/*.h
)
file(GLOB_RECURSE TEST_SOURCES tests/*.cpp)
list(REMOVE_ITEM SOURCES src/main.cpp)

# External Dependencies

# include_directories(dependencies/stb)
add_subdirectory(dependencies/stb)
add_subdirectory(dependencies/freetype)
add_subdirectory(dependencies/glfw)
add_subdirectory(dependencies/glad)
add_subdirectory(dependencies/cglm)

# Making Libraries and Executable

add_library(${PROJECT_LIB} ${SOURCES})
target_link_libraries(${PROJECT_LIB} stb)
target_link_libraries(${PROJECT_LIB} freetype)
target_link_libraries(${PROJECT_LIB} glfw)
target_link_libraries(${PROJECT_LIB} glad)
target_link_libraries(${PROJECT_LIB} cglm)
target_include_directories(${PROJECT_LIB} PUBLIC src)

add_executable(${PROJECT_NAME} src/main.cpp)
target_link_libraries(${PROJECT_NAME} ${PROJECT_LIB})

add_custom_command(
    TARGET ${PROJECT_NAME}
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:${PROJECT_NAME}> "../build"
    COMMAND ${CMAKE_COMMAND} -E rm $<TARGET_FILE:${PROJECT_NAME}>
    COMMENT "Copying EXECUTABLE to build folder"
)

# Tests

option(BUILD_TESTS "Build tests" OFF)

if(BUILD_TESTS)
    # Google Test Dependency
    add_subdirectory(dependencies/googletest)
    
    enable_testing()
    include(CTest)
    
    include_directories(src/)

    add_executable(${PROJECT_TESTS} ${TEST_SOURCES})
    target_link_libraries(${PROJECT_TESTS} gtest gtest_main)
    target_link_libraries(${PROJECT_TESTS} ${PROJECT_LIB})
    # include_directories(src)

    add_test(UnitTests ${PROJECT_TESTS})

    add_custom_command(
        TARGET ${PROJECT_TESTS}
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:${PROJECT_TESTS}> "../build"
        COMMAND ${CMAKE_COMMAND} -E rm $<TARGET_FILE:${PROJECT_TESTS}>
        COMMENT "Copying TESTS to build folder"
    )

endif()
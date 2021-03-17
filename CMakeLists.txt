cmake_minimum_required(VERSION 3.17)

# GLFW Dependency
add_subdirectory(dependencies/glfw)

add_subdirectory(dependencies/glad)

project(VoxelMade)

file(GLOB SOURCE  
    src/*.cpp
    src/*.h
)

add_executable(${PROJECT_NAME} ${SOURCE})

target_link_libraries(${PROJECT_NAME} glfw)
target_link_libraries(${PROJECT_NAME} glad)

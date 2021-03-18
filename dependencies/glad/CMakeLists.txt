cmake_minimum_required(VERSION 3.17)
project(glad)

include_directories(include/)

file(GLOB SOURCES
    "src/glad.c"
)

add_library(${PROJECT_NAME} STATIC ${SOURCES})

target_include_directories(${PROJECT_NAME} PUBLIC include/)
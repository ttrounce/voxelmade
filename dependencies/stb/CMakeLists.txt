cmake_minimum_required(VERSION 3.17)

project(stb)

add_library(${PROJECT_NAME} INTERFACE)
target_include_directories(${PROJECT_NAME} SYSTEM INTERFACE include/)
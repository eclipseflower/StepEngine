cmake_minimum_required(VERSION 3.5)

project(StepEngine)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

file(GLOB_RECURSE SRCS *.c *.cpp *.cc *.h *.hpp)

include_directories(${PROJECT_SOURCE_DIR})

add_executable(StepEngine ${SRCS})

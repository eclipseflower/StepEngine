project(StepEngine)

cmake_minimum_required(VERSION 3.5)

set(CMAKE_CXX_STANDARD 20)

file(GLOB SRCS *.c *.cpp *.cc *.h *.hpp)

add_executable(StepEngine ${SRCS})

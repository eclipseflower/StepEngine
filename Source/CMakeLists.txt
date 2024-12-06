cmake_minimum_required(VERSION 3.5)

project(StepEngine)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(FetchContent)
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG v1.15.2
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

file(GLOB_RECURSE SRCS *.c *.cpp *.cc *.h *.hpp)

include_directories(${PROJECT_SOURCE_DIR})

add_executable(StepEngine ${SRCS})

enable_testing()

target_link_libraries(StepEngine GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(StepEngine)

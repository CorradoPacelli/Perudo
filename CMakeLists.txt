cmake_minimum_required(VERSION 3.15)

project(Perudo VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(PROJECT_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include)

file(GLOB_RECURSE SOURCES 
    "src/*.cpp"
    "src/*.hpp"
)

add_executable(${PROJECT_NAME} ${SOURCES})

target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_INCLUDE_DIR})

if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /W4)
else()
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic)
endif()

# Google Test setup
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker on Windows
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

# Test executable
file(GLOB_RECURSE TEST_SOURCES 
    "test/*.cpp"
)

add_executable(${PROJECT_NAME}_test ${TEST_SOURCES} ${SOURCES})

target_include_directories(${PROJECT_NAME}_test PUBLIC ${PROJECT_INCLUDE_DIR})

target_link_libraries(${PROJECT_NAME}_test GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(${PROJECT_NAME}_test)
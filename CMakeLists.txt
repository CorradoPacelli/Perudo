cmake_minimum_required(VERSION 3.24)

project(Perudo VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Force compilers to use color output, even when Ninja buffers the output
set(CMAKE_COLOR_DIAGNOSTICS ON)

set(PROJECT_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include)

file(GLOB_RECURSE SOURCES 
    "src/*.cpp"
    "src/*.hpp"
)

list(FILTER SOURCES EXCLUDE REGEX "main\\.cpp$")

add_library(PerudoCore ${SOURCES})

target_include_directories(PerudoCore PUBLIC 
    ${PROJECT_INCLUDE_DIR}
    ${PROJECT_INCLUDE_DIR}/model
    ${PROJECT_INCLUDE_DIR}/model/actions
    ${PROJECT_INCLUDE_DIR}/model/states
    ${PROJECT_INCLUDE_DIR}/model/strategies
    ${PROJECT_INCLUDE_DIR}/controller
    ${PROJECT_INCLUDE_DIR}/view
)

add_executable(${PROJECT_NAME} src/main.cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE PerudoCore)

if(MSVC)
    target_compile_options(PerudoCore PRIVATE /W4 )
else()
    target_compile_options(PerudoCore PRIVATE -Wall -Wextra -Wpedantic -fdiagnostics-color=always)
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

file(GLOB_RECURSE TEST_SOURCES 
    "test/*.cpp"
)

add_executable(${PROJECT_NAME}_test ${TEST_SOURCES})
target_link_libraries(${PROJECT_NAME}_test PRIVATE PerudoCore GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(${PROJECT_NAME}_test)
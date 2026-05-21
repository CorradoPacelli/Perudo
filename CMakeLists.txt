cmake_minimum_required(VERSION 3.24)

project(Perudo VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_COLOR_DIAGNOSTICS ON)

set(PROJECT_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include)

file(GLOB_RECURSE SOURCES 
    "src/**/*.cpp"
    "src/**/*.hpp"
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
    ${PROJECT_INCLUDE_DIR}/common
)

add_executable(${PROJECT_NAME} src/main.cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE PerudoCore)

if(MSVC)
    target_compile_options(PerudoCore PRIVATE /W4 )
else()
    target_compile_options(PerudoCore PRIVATE -Wall -Wextra -Wpedantic -fdiagnostics-color=always)
endif()

# ASIO (Standalone) setup
include(FetchContent)
FetchContent_Declare(
  asio
  URL https://github.com/chriskohlhoff/asio/archive/asio-1-30-2.zip
)
FetchContent_MakeAvailable(asio)

target_include_directories(PerudoCore PUBLIC ${asio_SOURCE_DIR}/asio/include)
target_compile_definitions(PerudoCore PUBLIC ASIO_STANDALONE)

# Google Test setup
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker on Windows
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

file(GLOB_RECURSE TEST_SOURCES 
    "test/**/*.cpp"
)

add_executable(${PROJECT_NAME}_test ${TEST_SOURCES})
target_link_libraries(${PROJECT_NAME}_test PRIVATE PerudoCore GTest::gtest_main)

target_include_directories(${PROJECT_NAME}_test PRIVATE 
    ${PROJECT_SOURCE_DIR}/test/model
    ${PROJECT_SOURCE_DIR}/test/controller
    ${PROJECT_SOURCE_DIR}/test/common
)

include(GoogleTest)
gtest_discover_tests(${PROJECT_NAME}_test)

# Automatically run tests after the test executable is built.
add_custom_command(
    TARGET ${PROJECT_NAME}_test POST_BUILD
    COMMAND $<TARGET_FILE:${PROJECT_NAME}_test> --gtest_color=yes
    COMMENT "Running tests...")
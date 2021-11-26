cmake_minimum_required(VERSION 3.20)
project(P01)

set(CMAKE_CXX_STANDARD 20)


include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/1a3e2a265f35e604378156cd675b11ef0a184627.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_library(parser_lib STATIC parser/CalculatorParser.cpp parser/TokenStream.cpp parser/TokenStream.hpp parser/ParserException.cpp parser/ParserException.hpp)

add_executable(parser parser/parser.cpp)
target_link_libraries(parser parser_lib)

# Tests
enable_testing()
add_executable(parser_test test/parser_test.cpp)
target_link_libraries(parser_test gtest_main parser_lib)
target_include_directories(parser_test PRIVATE parser)
include(GoogleTest)
gtest_discover_tests(parser_test)

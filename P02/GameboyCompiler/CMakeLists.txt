cmake_minimum_required(VERSION 3.16)
project(GameboyCompiler)

set(CMAKE_CXX_STANDARD 20)

find_package(antlr4-generator REQUIRED)
find_package(antlr4-runtime REQUIRED)
find_package(fmt)

set(ANTLR4_JAR_LOCATION "/usr/local/lib/antlr-4.9.3-complete.jar" CACHE STRING "ANTLR antlr-4...-complete.jar path")

set(GENERATED_PARSER_NAME generated_gameboy_parser)

antlr4_generate(
        ${GENERATED_PARSER_NAME}
        ${CMAKE_CURRENT_SOURCE_DIR}/grammar/GameboyLanguage.g4
        BOTH # Parser/Lexer/Both
        FALSE # Listener
        TRUE # Visitor
        "gbparser" # Namespace
)

list(APPEND COMMON_FLAGS -Wall -Wextra -Wpedantic -Werror)
list(APPEND RELEASE_FLAGS ${COMMON_FLAGS} -O3)
list(APPEND DEBUG_FLAGS ${COMMON_FLAGS} -O0)

add_executable(GameboyCompiler main.cpp ${ANTLR4_SRC_FILES_${GENERATED_PARSER_NAME}} GameboyVisitor.cpp AST.cpp AssemblyOutput.cpp AssemblyOutput.hpp CodeGenerator.cpp CodeGenerator.hpp)
target_link_libraries(GameboyCompiler PRIVATE antlr4_static fmt::fmt)
target_include_directories(GameboyCompiler PRIVATE ${ANTLR4_INCLUDE_DIR} ${ANTLR4_INCLUDE_DIR_${GENERATED_PARSER_NAME}})


if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif ()

if (CMAKE_BUILD_TYPE MATCHES Release)
    target_compile_options(GameboyCompiler
            PRIVATE
            ${RELEASE_FLAGS}
            )
elseif (CMAKE_BUILD_TYPE MATCHES Debug)
    target_compile_options(GameboyCompiler
            PRIVATE
            ${DEBUG_FLAGS}
            )
endif ()

#add_subdirectory(tests)

cmake_minimum_required(VERSION 3.16)
project(GameboyCompiler)

set(CMAKE_CXX_STANDARD 20)

find_package(antlr4-generator REQUIRED)
find_package(antlr4-runtime REQUIRED)

set(ANTLR4_JAR_LOCATION "/usr/local/lib/antlr-4.9.3-complete.jar" CACHE STRING "ANTLR antlr-4...-complete.jar path")

antlr4_generate(
        deliveryparser
        ${CMAKE_CURRENT_SOURCE_DIR}/grammar/Delivery.g4
        BOTH # Parser/Lexer/Both
        FALSE # Listener
        TRUE # Visitor
        "delivery" # Namespace
)

add_executable(delivery delivery.cpp ${ANTLR4_SRC_FILES_deliveryparser} DeliveryVisitor.cpp)
target_link_libraries(delivery PRIVATE antlr4_static)
target_include_directories(delivery PRIVATE ${ANTLR4_INCLUDE_DIR} ${ANTLR4_INCLUDE_DIR_deliveryparser})

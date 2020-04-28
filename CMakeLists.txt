cmake_minimum_required(VERSION 3.3)
project(navigation-system-prototype VERSION 1.0)
set(CMAKE_BUILD_TYPE Release)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
include_directories(include)
find_package(FLEX)
FLEX_TARGET(MyScanner lexer.l ${CMAKE_CURRENT_BINARY_DIR}/lex.json.cc)

file(GLOB SOURCES
    "include/nsp/*.h"
    "src/*.cpp"
    "include/nsp/lex.json.cc"
)
add_executable(nspBin ${SOURCES})
#add_subdirectory(test)

cmake_minimum_required(VERSION 3.3)
include_directories(${PROJECT_SOURCE_DIR}/build/lib/)
add_executable(nspTest main.c)
target_link_libraries(nspTest PUBLIC ${PROJECT_SOURCE_DIR}/build/libnsp.a cppunit)

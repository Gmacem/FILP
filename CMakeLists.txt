project(FILP)

cmake_minimum_required(VERSION 3.8)

set(CMAKE_CXX_STANDARD             17)
set(CMAKE_MODULE_PATH              "${CMAKE_SOURCE_DIR}/cmake")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")
set(CMAKE_EXPORT_COMPILE_COMMANDS  ON)

find_package(Catch2 REQUIRED)
#target_link_libraries(tests Catch2::Catch2)

add_subdirectory(word_counter)
add_subdirectory(smart_pointer)

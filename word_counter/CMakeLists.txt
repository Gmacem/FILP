add_library(lib
    reader.cpp
    parser.cpp
    stat.cpp)

add_executable(word-counter main.cpp)

target_link_libraries(word-counter PRIVATE lib)

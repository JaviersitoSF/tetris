cmake_minimum_required(VERSION 4.2.1)

# Library target for tetris
add_library(libtetris STATIC
    tetris/tetris.cpp
    tetris/board.cpp
)

target_include_directories(libtetris PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/tetris)

# Executable
add_executable(tetris main.cpp)

target_link_libraries(tetris PRIVATE libtetris)

target_compile_features(tetris PRIVATE cxx_std_17)

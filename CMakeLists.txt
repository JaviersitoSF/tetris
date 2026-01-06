cmake_minimum_required(VERSION 4.2.1)
project(tetris_project VERSION 0.1 LANGUAGES CXX)

# Enable testing at top-level so CTest discovers tests in subdirectories
enable_testing()

# Put all sources under src/
add_subdirectory(src)

# Add tests directory (contains CTest-registered executables)
add_subdirectory(tests)

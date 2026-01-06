cmake_minimum_required(VERSION 4.2.1)
project(tetris_tests)

enable_testing()

add_executable(test_basic test_basic.cpp)
# include project headers
target_include_directories(test_basic PRIVATE ${PROJECT_SOURCE_DIR}/src)
# use C++17
target_compile_features(test_basic PRIVATE cxx_std_17)
# link with library if needed
target_link_libraries(test_basic PRIVATE libtetris)

add_test(NAME basic_test COMMAND test_basic)

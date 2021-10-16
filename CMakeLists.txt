cmake_minimum_required(VERSION 3.20)
project(circuits)

enable_testing()

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 17)

add_compile_options(-g -Wall -Wextra -Werror)

add_subdirectory(external/googletest)
add_test(tennis_test tennis_test)

add_executable(matrix_test t/matrix_test.cpp matrix.cpp matrix.h)
add_executable(circuit main.cpp matrix.cpp graph.cpp graph.h circuit.cpp circuit.h)
target_link_libraries(matrix_test gtest gmock gmock_main)
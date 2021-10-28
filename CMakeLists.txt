cmake_minimum_required(VERSION 3.16)
project(circuits)

enable_testing()

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 17)

add_compile_options(-g -Wall -Wextra -Werror)

add_subdirectory(external/googletest)
add_test(matrix_test matrix_test)
add_test(graph_test graph_test)
add_test(input_parser_test input_parser_test)
add_test(circuit_test circuit_test)
add_test(big_circuit_test big_circuit_test)

add_executable(matrix_test t/matrix_test.cpp matrix.cpp matrix.h)
add_executable(graph_test t/graph_test.cpp graph.cpp graph.h)
add_executable(input_parser_test t/input_parser_test.cpp input_parser.cpp input_parser.h)
add_executable(circuit_test t/circuit_test.cpp circuit.cpp circuit.h input_parser.cpp input_parser.h graph.cpp graph.h matrix.cpp matrix.h)
add_executable(big_circuit_test t/big_circuit_test.cpp circuit.cpp circuit.h input_parser.cpp input_parser.h graph.cpp graph.h matrix.cpp matrix.h)
add_executable(circuit main.cpp matrix.cpp graph.cpp graph.h circuit.cpp circuit.h input_parser.cpp input_parser.h)
target_link_libraries(matrix_test gtest gmock gmock_main)
target_link_libraries(graph_test gtest gmock gmock_main)
target_link_libraries(input_parser_test gtest gmock gmock_main)
target_link_libraries(circuit_test gtest gmock gmock_main)
target_link_libraries(big_circuit_test gtest gmock gmock_main)
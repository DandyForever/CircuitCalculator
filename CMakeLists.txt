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
add_test(dc_circuit_test dc_circuit_test)
add_test(ac_circuit_test dc_circuit_test)
add_test(big_dc_circuit_test big_dc_circuit_test)
add_test(big_ac_circuit_test big_ac_circuit_test)

set(GTEST gtest gmock gmock_main)
set(SOURCE circuit.cpp input_parser.cpp graph.cpp matrix.cpp)

add_executable(matrix_test t/matrix_test.cpp matrix.cpp)
add_executable(graph_test t/graph_test.cpp graph.cpp)
add_executable(input_parser_test t/input_parser_test.cpp input_parser.cpp)
add_executable(dc_circuit_test t/dc_circuit_test.cpp ${SOURCE})
add_executable(ac_circuit_test t/ac_circuit_test.cpp ${SOURCE})
add_executable(big_dc_circuit_test t/big_dc_circuit_test.cpp ${SOURCE})
add_executable(big_ac_circuit_test t/big_ac_circuit_test.cpp ${SOURCE})
add_executable(dc_circuit dc_circuit.cpp ${SOURCE})
add_executable(ac_circuit ac_circuit.cpp ${SOURCE})
target_link_libraries(matrix_test ${GTEST})
target_link_libraries(graph_test ${GTEST})
target_link_libraries(input_parser_test ${GTEST})
target_link_libraries(dc_circuit_test ${GTEST})
target_link_libraries(ac_circuit_test ${GTEST})
target_link_libraries(big_dc_circuit_test ${GTEST})
target_link_libraries(big_ac_circuit_test ${GTEST})
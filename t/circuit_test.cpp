#include <gtest/gtest.h>

#include "../circuit.h"

TEST(CalculationTest, CalculationTest1) {
    std::string inp("1 -- 2, 4.0; 1 -- 3, 10.0; 1 -- 4, 2.0; -12.0V; 2 -- 3, 60.0; 2 -- 4, 22.0; 3 -- 4, 5.0;");
    circuit test_circuit(inp);
    test_circuit.calculate_edge_current();
    std::string answer("1 -- 2: -0.442958 A; 1 -- 3: -0.631499 A; 1 -- 4: 1.07446 A; 2 -- 3: -0.0757193 A; 2 -- 4: -0.367239 A; 3 -- 4: -0.707219 A;");
    EXPECT_EQ(answer, test_circuit.get_edge_current_answer());
}

TEST(CalculationTest, ConsistentCircuitTest1) {
    std::string inp("1 -- 2, 0.5; 50V; 2 -- 3, 20; 3 -- 1, 32;");
    circuit test_circuit(inp);
    test_circuit.calculate_edge_current();
    std::string answer("1 -- 2: -0.952381 A; 2 -- 3: -0.952381 A; 3 -- 1: -0.952381 A;");
    EXPECT_EQ(answer, test_circuit.get_edge_current_answer());
}

TEST(CalculationTest, LongCircuitTest1) {
    std::string inp("1 -- 2, 3; 15V; 2 -- 3, 1; 2 -- 4, 10; 3 -- 4, 5; 3 -- 4, 10; 4 -- 1, 2;");
    circuit test_circuit(inp);
    test_circuit.calculate_edge_current();
    std::string answer("1 -- 2: -1.86957 A; 2 -- 3: -1.30435 A; 2 -- 4: -0.565217 A; 3 -- 4: -0.869565 A; 3 -- 4: -0.434783 A; 4 -- 1: -1.86957 A;");
    EXPECT_EQ(answer, test_circuit.get_edge_current_answer());
}

TEST(CalculationTest, TwoEMFCodirectionalConsistentTest1) {
    std::string inp("1 -- 2, 30; 10V; 2 -- 3, 20; 3 -- 1; 10; 2V;");
    circuit test_circuit(inp);
    test_circuit.calculate_edge_current();
    std::string answer("1 -- 2: -0.2 A; 2 -- 3: -0.2 A; 3 -- 1: -0.2 A;");
    EXPECT_EQ(answer, test_circuit.get_edge_current_answer());
}

TEST(CalculationTest, TwoEMFCounterdirectionalConsistentTest1) {
    std::string inp("1 -- 2, 30; 10V; 2 -- 3, 20; 3 -- 1; 10; -2V;");
    circuit test_circuit(inp);
    test_circuit.calculate_edge_current();
    std::string answer("1 -- 2: -0.133333 A; 2 -- 3: -0.133333 A; 3 -- 1: -0.133333 A;");
    EXPECT_EQ(answer, test_circuit.get_edge_current_answer());
}


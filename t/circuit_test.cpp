#include <gtest/gtest.h>

#include "../circuit.h"

TEST(CalculationTest, CalculationTest1) {
    std::string inp("1 -- 2, 4.0; 1 -- 3, 10.0; 1 -- 4, 2.0; -12.0V; 2 -- 3, 60.0; 2 -- 4, 22.0; 3 -- 4, 5.0;");
    circuit test_circuit(inp);
    test_circuit.calculate_edge_current();
    std::string answer("1 -- 2: -0.442958 A; 1 -- 3: -0.631499 A; 1 -- 4: 1.07446 A; 2 -- 3: -0.0757193 A; 2 -- 4: -0.367239 A; 3 -- 4: -0.707219 A;");
    EXPECT_EQ(answer, test_circuit.get_edge_current_answer());
}

TEST(CalculationTest, ConsistentCircuitTest) {
    std::string inp("1 -- 2, 0.5; 50V; 2 -- 3, 20; 3 -- 1, 32;");
    circuit test_circuit(inp);
    test_circuit.calculate_edge_current();
    std::string answer("1 -- 2: -0.952381 A; 2 -- 3: -0.952381 A; 3 -- 1: -0.952381 A;");
    EXPECT_EQ(answer, test_circuit.get_edge_current_answer());
}

TEST(CalculationTest, LongCircuitTest) {
    std::string inp("1 -- 2, 3; 15V; 2 -- 3, 1; 2 -- 4, 10; 3 -- 4, 5; 3 -- 4, 10; 4 -- 1, 2;");
    circuit test_circuit(inp);
    test_circuit.calculate_edge_current();
    std::string answer("1 -- 2: -1.86957 A; 2 -- 3: -1.30435 A; 2 -- 4: -0.565217 A; 3 -- 4: -0.869565 A; 3 -- 4: -0.434783 A; 4 -- 1: -1.86957 A;");
    EXPECT_EQ(answer, test_circuit.get_edge_current_answer());
}

TEST(CalculationTest, TwoEMFCodirectionalConsistentTest) {
    std::string inp("1 -- 2, 30; 10V; 2 -- 3, 20; 3 -- 1; 10; 2V;");
    circuit test_circuit(inp);
    test_circuit.calculate_edge_current();
    std::string answer("1 -- 2: -0.2 A; 2 -- 3: -0.2 A; 3 -- 1: -0.2 A;");
    EXPECT_EQ(answer, test_circuit.get_edge_current_answer());
}

TEST(CalculationTest, TwoEMFCounterdirectionalConsistentTest) {
    std::string inp("1 -- 2, 30; 10V; 2 -- 3, 20; 3 -- 1; 10; -2V;");
    circuit test_circuit(inp);
    test_circuit.calculate_edge_current();
    std::string answer("1 -- 2: -0.133333 A; 2 -- 3: -0.133333 A; 3 -- 1: -0.133333 A;");
    EXPECT_EQ(answer, test_circuit.get_edge_current_answer());
}

TEST(CalculationTest, NoVoltageTest) {
    std::string inp("1 -- 2, 30; 2 -- 3, 20; 3 -- 1; 10;");
    circuit test_circuit(inp);
    test_circuit.calculate_edge_current();
    std::string answer("1 -- 2: -0 A; 2 -- 3: -0 A; 3 -- 1: -0 A;");
    EXPECT_EQ(answer, test_circuit.get_edge_current_answer());
}

TEST(CalculationTest, TwoEMFCodirectionalParallelTest) {
    std::string inp("1 -- 2, 5; 6.0V; 2 -- 3, 10; -30.0V; 2 -- 4, 5; 4 -- 3, 5; 3 -- 1, 5;");
    circuit test_circuit(inp);
    test_circuit.calculate_edge_current();
    std::string answer("1 -- 2: 0.6 A; 2 -- 3: 1.8 A; 2 -- 4: -1.2 A; 4 -- 3: -1.2 A; 3 -- 1: 0.6 A;");
    EXPECT_EQ(answer, test_circuit.get_edge_current_answer());
}

TEST(CalculationTest, TwoEMFCounterdirectionalParallelTest) {
    std::string inp("1 -- 2, 5; 6.0V; 2 -- 3, 10; 30.0V; 2 -- 4, 5; 4 -- 3, 5; 3 -- 1, 5;");
    circuit test_circuit(inp);
    test_circuit.calculate_edge_current();
    std::string answer("1 -- 2: -1.4 A; 2 -- 3: -2.2 A; 2 -- 4: 0.8 A; 4 -- 3: 0.8 A; 3 -- 1: -1.4 A;");
    EXPECT_EQ(answer, test_circuit.get_edge_current_answer());
}

TEST(CalculationTest, CalculationTest2) {
    std::string inp("1 -- 2, 50; -75.0V; 2 -- 3, 50; 1 -- 3, 150; 1 -- 4, 100; -100.0V; 4 -- 3, 50;");
    circuit test_circuit(inp);
    test_circuit.calculate_edge_current();
    std::string answer("1 -- 2: 0.142857 A; 2 -- 3: 0.142857 A; 1 -- 3: -0.404762 A; 1 -- 4: 0.261905 A; 4 -- 3: 0.261905 A;");
    EXPECT_EQ(answer, test_circuit.get_edge_current_answer());
}

TEST(CalculationTest, CalculationTest3) {
    std::string inp("1 -- 2, 50; -75.0V; 2 -- 3, 50; 1 -- 3, 150; 1 -- 4, 100; 100.0V; 4 -- 3, 50;");
    circuit test_circuit(inp);
    test_circuit.calculate_edge_current();
    std::string answer("1 -- 2: 0.714286 A; 2 -- 3: 0.714286 A; 1 -- 3: -0.0238095 A; 1 -- 4: -0.690476 A; 4 -- 3: -0.690476 A;");
    EXPECT_EQ(answer, test_circuit.get_edge_current_answer());
}

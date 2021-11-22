#include <gtest/gtest.h>

#include "../circuit.h"

void generate_calculation_test(const std::string& inp, const std::string& answer) {
    std::stringstream input(inp), output;
    calculate_circuit<double>(input, output);
    EXPECT_EQ(answer, output.str());
}

TEST(CalculationTest, CalculationTest1) {
    std::string inp("1 -- 2, 4.0; 1 -- 3, 10.0; 1 -- 4, 2.0; -12.0V; 2 -- 3, 60.0; 2 -- 4, 22.0; 3 -- 4, 5.0;");
    std::string answer("1 -- 2: 0.442958 A;\n1 -- 3: 0.631499 A;\n1 -- 4: -1.07446 A;\n2 -- 3: 0.0757193 A;\n2 -- 4: 0.367239 A;\n3 -- 4: 0.707219 A;");
    generate_calculation_test(inp, answer);
}

TEST(CalculationTest, ConsistentCircuitTest) {
    std::string inp("1 -- 2, 0.5; 50V; 2 -- 3, 20; 3 -- 1, 32;");
    std::string answer("1 -- 2: 0.952381 A;\n2 -- 3: 0.952381 A;\n3 -- 1: 0.952381 A;");
    generate_calculation_test(inp, answer);
}

TEST(CalculationTest, LongCircuitTest) {
    std::string inp("1 -- 2, 3; 15V; 2 -- 3, 1; 2 -- 4, 10; 3 -- 4, 5; 3 -- 4, 10; 4 -- 1, 2;");
    std::string answer("1 -- 2: 1.86957 A;\n2 -- 3: 1.30435 A;\n2 -- 4: 0.565217 A;\n3 -- 4: 0.869565 A;\n3 -- 4: 0.434783 A;\n4 -- 1: 1.86957 A;");
    generate_calculation_test(inp, answer);
}

TEST(CalculationTest, TwoEMFCodirectionalConsistentTest) {
    std::string inp("1 -- 2, 30; 10.V; 2 -- 3, 20.; 3 -- 1, 10.; 2.V;");
    std::string answer("1 -- 2: 0.2 A;\n2 -- 3: 0.2 A;\n3 -- 1: 0.2 A;");
    generate_calculation_test(inp, answer);
}

TEST(CalculationTest, TwoEMFCounterdirectionalConsistentTest) {
    std::string inp("1 -- 2, 30; 10.V; 2 -- 3, 20.; 3 -- 1, 10.; -2.V;");
    std::string answer("1 -- 2: 0.133333 A;\n2 -- 3: 0.133333 A;\n3 -- 1: 0.133333 A;");
    generate_calculation_test(inp, answer);
}

TEST(CalculationTest, NoVoltageTest) {
    std::string inp("1 -- 2, 30; 2 -- 3, 20; 3 -- 1, 10;");
    std::string answer("1 -- 2: 0 A;\n2 -- 3: 0 A;\n3 -- 1: 0 A;");
    generate_calculation_test(inp, answer);
}

TEST(CalculationTest, TwoEMFCodirectionalParallelTest) {
    std::string inp("1 -- 2, 5; 6.0V; 2 -- 3, 10; -30.0V; 2 -- 4, 5; 4 -- 3, 5; 3 -- 1, 5;");
    std::string answer("1 -- 2: -0.6 A;\n2 -- 3: -1.8 A;\n2 -- 4: 1.2 A;\n3 -- 1: -0.6 A;\n4 -- 3: 1.2 A;");
    generate_calculation_test(inp, answer);
}

TEST(CalculationTest, TwoEMFCounterdirectionalParallelTest) {
    std::string inp("1 -- 2, 5; 6.0V; 2 -- 3, 10; 30.0V; 2 -- 4, 5; 4 -- 3, 5; 3 -- 1, 5;");
    std::string answer("1 -- 2: 1.4 A;\n2 -- 3: 2.2 A;\n2 -- 4: -0.8 A;\n3 -- 1: 1.4 A;\n4 -- 3: -0.8 A;");
    generate_calculation_test(inp, answer);
}

TEST(CalculationTest, CalculationTest2) {
    std::string inp("1 -- 2, 50; -75.0V; 2 -- 3, 50; 1 -- 3, 150; 1 -- 4, 100; -100.0V; 4 -- 3, 50;");
    std::string answer("1 -- 2: -0.142857 A;\n1 -- 3: 0.404762 A;\n1 -- 4: -0.261905 A;\n2 -- 3: -0.142857 A;\n4 -- 3: -0.261905 A;");
    generate_calculation_test(inp, answer);
}

TEST(CalculationTest, CalculationTest3) {
    std::string inp("1 -- 2, 50; -75.0V; 2 -- 3, 50; 1 -- 3, 150; 1 -- 4, 100; 100.0V; 4 -- 3, 50;");
    std::string answer("1 -- 2: -0.714286 A;\n1 -- 3: 0.0238095 A;\n1 -- 4: 0.690476 A;\n2 -- 3: -0.714286 A;\n4 -- 3: 0.690476 A;");
    generate_calculation_test(inp, answer);
}

TEST(CircuitThrowsZeroResistanceExceptionTest, ZeroResistanceTest) {
    std::string inp("1 -- 2, 0.0;");
    std::string answer("1 -- 2: 0 A;");
    generate_calculation_test(inp, answer);
}

TEST(IncoherentCircuitTest, IncoherentSimpleCircuitTest) {
    std::string inp("1 -- 1, 1.0; 2.0V; 2 -- 2, 1.0; 2.0V;");
    std::string answer("1 -- 1: 2 A;\n2 -- 2: 2 A;");
    generate_calculation_test(inp, answer);
}

TEST(IncoherentCircuitTest, IncoherentCircuitTest) {
    std::string inp("1 -- 2, 1.0; 2.0V; 2 -- 1, 1.0; 2.0V; 3 -- 4, 1.0; 2.0V; 4 -- 3, 1.0; 2.0V");
    std::string answer("1 -- 2: 2 A;\n2 -- 1: 2 A;\n3 -- 4: 2 A;\n4 -- 3: 2 A;");
    generate_calculation_test(inp, answer);
}

TEST(IncoherentCircuitTest, NotAllVerticesInInputTest) {
    std::string inp("2 -- 3, 1.0;");
    std::string answer("2 -- 3: 0 A;");
    generate_calculation_test(inp, answer);
}

TEST(CalculationLoopsTest, CalculationLoopsTrivialTest) {
    std::string inp("1 -- 1, 1.0; 2.0V;");
    std::string answer("1 -- 1: 2 A;");
    generate_calculation_test(inp, answer);
}

TEST(CalculationLoopsTest, CalculationLoopsTest1) {
    std::string inp("1 -- 1, 1.0; 2.0V; 2 -- 2, 1.0; 2.0V; 1 -- 2, 1.0; 2.0V");
    std::string answer("1 -- 1: 2 A;\n2 -- 2: 2 A;\n1 -- 2: 0 A;");
    generate_calculation_test(inp, answer);
}

TEST(CalculationLoopsTest, CalculationLoopsTest2) {
    std::string inp("1 -- 1, 1.0; 2.0V; 2 -- 2, 1.0; 2.0V; 1 -- 2, 1.0; 2.0V; 2 -- 1, 1.0;");
    std::string answer("1 -- 1: 2 A;\n2 -- 2: 2 A;\n1 -- 2: 1 A;\n2 -- 1: 1 A;");
    generate_calculation_test(inp, answer);
}

TEST(CalculationTest, OpenCircuitTest1) {
    std::string inp("1 -- 2, 1.0; 2.0V; 2 -- 3, 1.0; 2.0V; 3 -- 2, 1.0; 2.0V");
    std::string answer("1 -- 2: 0 A;\n2 -- 3: 2 A;\n3 -- 2: 2 A;");
    generate_calculation_test(inp, answer);
}

TEST(CalculationTest, OpenCircuitTest2) {
    std::string inp("1 -- 2, 1.0; 2 -- 3, 1.0; 2.0V; 3 -- 2, 1.0; 2.0V");
    std::string answer("1 -- 2: 0 A;\n2 -- 3: 2 A;\n3 -- 2: 2 A;");
    generate_calculation_test(inp, answer);
}

TEST(CalculationTest, OpenCircuitTest3) {
    std::string inp("1 -- 2, 1.0; 2 -- 3, 1.0; 3 -- 2, 1.0; 2.0V");
    std::string answer("1 -- 2: 0 A;\n2 -- 3: 1 A;\n3 -- 2: 1 A;");
    generate_calculation_test(inp, answer);
}

TEST(CalculationTest, OpenCircuitTest4) {
    std::string inp("1 -- 2, 1.0; 2 -- 3, 1.0; 3 -- 2, 1.0;");
    std::string answer("1 -- 2: 0 A;\n2 -- 3: 0 A;\n3 -- 2: 0 A;");
    generate_calculation_test(inp, answer);
}

TEST(CalculationTest, OpenCircuitTest5) {
    std::string inp("1 -- 2, 1.0; 2 -- 1, 1.0; 3 -- 2, 1.0; 2.0V");
    std::string answer("1 -- 2: 0 A;\n2 -- 1: 0 A;\n3 -- 2: 0 A;");
    generate_calculation_test(inp, answer);
}

TEST(CalculationTest, OpenCircuitTest6) {
    std::string inp("1 -- 2, 1.0; 2.0V; 2 -- 1, 1.0; 3 -- 2, 1.0; 2.0V");
    std::string answer("1 -- 2: 1 A;\n2 -- 1: 1 A;\n3 -- 2: 0 A;");
    generate_calculation_test(inp, answer);
}

TEST(CalculationTest, OpenCircuitTest7) {
    std::string inp("1 -- 2, 1.0; 2 -- 1, 1.0; 2.0V; 3 -- 2, 1.0; 2.0V");
    std::string answer("1 -- 2: 1 A;\n2 -- 1: 1 A;\n3 -- 2: 0 A;");
    generate_calculation_test(inp, answer);
}

TEST(CalculationTest, FullyOpenCircuitTest) {
    std::string inp("1 -- 2, 1.0; 2 -- 3, 1.0; 2.0V; 4 -- 2, 1.0; 2.0V");
    std::string answer("1 -- 2: 0 A;\n2 -- 3: 0 A;\n4 -- 2: 0 A;");
    generate_calculation_test(inp, answer);
}

TEST(CalculationTest, OpenCircuitDifficultTest) {
    std::string inp("1 -- 2, 4.0; 1 -- 3, 10.0; 1 -- 4, 2.0; -12.0V; 2 -- 3, 60.0; 2 -- 4, 22.0; 3 -- 4, 5.0; 4 -- 5, 1.0; 100.0V;");
    std::string answer("1 -- 2: 0.442958 A;\n1 -- 3: 0.631499 A;\n1 -- 4: -1.07446 A;\n2 -- 3: 0.0757193 A;\n2 -- 4: 0.367239 A;\n3 -- 4: 0.707219 A;\n4 -- 5: -2.84217e-14 A;");
    generate_calculation_test(inp, answer);
}

TEST(CalculationLoopsTest, CalculationLoopsDifficultTest) {
    std::string inp("1 -- 2, 4.0; 1 -- 3, 10.0; 1 -- 4, 2.0; -12.0V; 2 -- 3, 60.0; 2 -- 4, 22.0; 3 -- 4, 5.0; 4 -- 4, 1.0; 100.0V;");
    std::string answer("4 -- 4: 100 A;\n1 -- 2: 0.442958 A;\n1 -- 3: 0.631499 A;\n1 -- 4: -1.07446 A;\n2 -- 3: 0.0757193 A;\n2 -- 4: 0.367239 A;\n3 -- 4: 0.707219 A;");
    generate_calculation_test(inp, answer);
}

TEST(CalculationTest, CalculationZeroResistanceDifficultTest) {
    std::string inp("1 -- 2, 1; 1 -- 6, 0; 2 -- 3, 2; 2 -- 7, 2; 3 -- 4, 2; 3 -- 8, 2; 4 -- 5, 1; 4 -- 9, 2; "
                    "5 -- 10, 0; 6 -- 7, 1; 6 -- 11, 0; 7 -- 8, 2; 7 -- 12, 2; 8 -- 9, 2; 8 -- 13, 2; 9 -- 10, 1;"
                    " 9 -- 14, 2; 10 -- 15, 0; 11 -- 12, 1; 12 -- 13, 2; 13 -- 14, 2; 14 -- 15, 1; 11 -- 16, 0.0;"
                    " 15 -- 20, 0.0; 20 -- 16, 0.0; 120 V");
    std::string answer("1 -- 2: 20 A;\n1 -- 6: -20 A;\n2 -- 3: 20 A;\n2 -- 7: -5.83329e-09 A;\n3 -- 4: 20 A;\n"
                       "3 -- 8: -1.06581e-14 A;\n4 -- 5: 20 A;\n4 -- 9: 5.83333e-09 A;\n5 -- 10: 20 A;\n6 -- 7: 20 A;\n"
                       "6 -- 11: -40.0001 A;\n7 -- 8: 20 A;\n7 -- 12: -9.16673e-09 A;\n8 -- 9: 20 A;\n"
                       "8 -- 13: -1.06581e-14 A;\n9 -- 10: 20 A;\n9 -- 14: 9.16665e-09 A;\n10 -- 15: 40 A;\n"
                       "11 -- 12: 20 A;\n11 -- 16: -60 A;\n12 -- 13: 20 A;\n13 -- 14: 20 A;\n14 -- 15: 20 A;\n"
                       "15 -- 20: 60 A;\n20 -- 16: 60 A;");
    generate_calculation_test(inp, answer);
}

TEST(EmptyInputTest, EmptyInputTest) {
    std::string inp("");
    EXPECT_THROW(circuit<double> test_circuit(inp), circuit<double>::EmptyInputException);
}

TEST(CircuitThrowsZeroVerticesExceptionTest, ZeroVerticesTest) {
    std::string inp("0 -- 0, 4.0; 2.0V;");
    EXPECT_THROW(circuit<double> test_circuit(inp), circuit<double>::ZeroVerticesException);
}

TEST(CircuitThrowsZeroVerticesExceptionTest, ZeroVertexTest) {
    std::string inp("0 -- 1, 4.0; 2.0V;");
    EXPECT_THROW(circuit<double> test_circuit(inp), circuit<double>::ZeroVerticesException);
}

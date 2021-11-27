#include <gtest/gtest.h>

#include "../circuit.h"

void generate_calculation_test(const std::string& inp, const std::string& answer) {
    std::stringstream input(inp), output;
    calculate_circuit<std::complex<double>>(input, output);
    EXPECT_EQ(answer, output.str());
}

TEST(ACCircutTest, ResonanceCaseTest) {
    std::string inp("1 -- 2, 0R; 1C; 0L; 2 -- 1, 0R; 0C; 1L;");
    std::string answer("1 -- 2: 0 A, 0;\n2 -- 1: 0 A, 0;");
    generate_calculation_test(inp, answer);
}

TEST(ACCircuitTest, ConsistentCapacityTest) {
    std::string inp("1 -- 2, 0R; 1C; 0L; 1V, 0; 2 -- 1, 0R; 1C; 0L;");
    std::string answer("1 -- 2: 0.5 A, 90;\n2 -- 1: 0.5 A, 90;");
    generate_calculation_test(inp, answer);
}

TEST(ACCircuitTest, ParallelCapacityTest) {
    std::string inp("1 -- 2, 0R; 1C; 0L; 1 -- 2, 0R; 1C; 0L; 2 -- 1, 0R; 2C; 0L; 4V, 0;");
    std::string answer("1 -- 2: 0.8 A, 90;\n1 -- 2: 0.8 A, 90;\n2 -- 1: 1.6 A, 90;");
    generate_calculation_test(inp, answer);
}

TEST(ACCircuitTest, ConsistentConductanceTest) {
    std::string inp("1 -- 2, 0R; 0C; 1L; 1V, 0; 2 -- 1, 0R; 0C; 1L;");
    std::string answer("1 -- 2: 0.5 A, -90;\n2 -- 1: 0.5 A, -90;");
    generate_calculation_test(inp, answer);
}

TEST(ACCircuitTest, ParallelConductanceTest) {
    std::string inp("1 -- 2, 0R; 0C; 2L; 1 -- 2, 0R; 0C; 2L; 2 -- 1, 0R; 0C; 1L; 2V, 0;");
    std::string answer("1 -- 2: 0.5 A, -90;\n1 -- 2: 0.5 A, -90;\n2 -- 1: 1 A, -90;");
    generate_calculation_test(inp, answer);
}

TEST(ACCircuitTest, CalculationTest1) {
    std::string inp("1 -- 1, 20R; 106.1C; 0.94L; 50V, 0;");
    std::string answer("1 -- 1: 0.467093 A, 79.2317;");
    generate_calculation_test(inp, answer);
}

TEST(ACCircuitTest, CalculationTest2) {
    std::string inp("1 -- 2, 4.0R; 0C; 0L; 1 -- 3, 10.0R; 0C; 0L; 1 -- 4, 2.0R; 0C; 0L; -12.0V, 0; 2 -- 3, 60.0R; 0C; 0L; 2 -- 4, 22.0R; 0C; 0L; 3 -- 4, 5.0R; 0C; 0L;");
    std::string answer("1 -- 2: 0.442958 A, 0;\n1 -- 3: 0.631499 A, 0;\n1 -- 4: 1.07446 A, 180;\n2 -- 3: 0.0757193 A, 0;\n2 -- 4: 0.367239 A, 0;\n3 -- 4: 0.707219 A, 0;");
    generate_calculation_test(inp, answer);
}


TEST(ACCircuitTest, CalculationTest3) {
    std::string inp("1 -- 2, 50R; 0C; 50L; 100V, 0; 2 -- 1, 100R; 50C; 0L; 2 -- 1, 100R; 0C; 100L;");
    std::string answer("1 -- 2: 0.723241 A, -23.8387;\n2 -- 1: 0.496139 A, 7.12502;\n2 -- 1: 0.392232 A, -64.44;");
    generate_calculation_test(inp, answer);
}

TEST(ACCircuitTest, CalculationTest4) {
    std::string inp("1 -- 2, 0R; 0C; 10L; 141V, 0; 2 -- 1, 0R; 10C; 0L; 2 -- 1, 10R; 0C; 0L;");
    std::string answer("1 -- 2: 19.9404 A, -45;\n2 -- 1: 14.1 A, 0;\n2 -- 1: 14.1 A, -90;");
    generate_calculation_test(inp, answer);
}

TEST(ACCircuitTest, CalculationTest5) {
    std::string inp("1 -- 1, 12R; 20C; 36L; 100V, 0;");
    std::string answer("1 -- 1: 5 A, -53.1301;");
    generate_calculation_test(inp, answer);
}

TEST(ACCircuitTest, CalculationTest6) {
    std::string inp("1 -- 2, 12R; 0C; 0L; 100V, 0; 2 -- 3, 0R; 20C; 0L; 3 -- 1, 0R; 0C; 36L;");
    std::string answer("1 -- 2: 5 A, -53.1301;\n2 -- 3: 5 A, -53.1301;\n3 -- 1: 5 A, -53.1301;");
    generate_calculation_test(inp, answer);
}
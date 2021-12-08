#include <gtest/gtest.h>
#include <fstream>

#include "../circuit.h"

void generate_calculation_test(const std::string& inp, const std::string& answer) {
    std::stringstream input(inp), output;
    calculate_complex_circuit<std::complex<double>>(input, "test", output);
    EXPECT_EQ(answer, output.str());
}

void generate_test_file(const std::string& file_name, const std::string& file_contents) {
    std::ofstream file;
    file.open(file_name);
    file << file_contents;
    file.close();
}

TEST(CalculationTest, DCCircuit1Test) {
    generate_test_file("dc_test.txt",
                       "define DCCircuit(A, B, C, D)\n A -- B, 4.0R;0C;0L;\n A -- C, 10.0R;0C;0L;\n A -- D, 2.0R;0C;0L; -12.0V, 0;\n B -- C, 60.0R;0C;0L;\n B -- D, 22.0R;0C;0L;\n C -- D, 5.0R;0C;0L;");
    generate_calculation_test("include dc_test.txt\nDCCircuit(1, 2, 3, 4)",
                              "1 -- 2: 0.442958 A, 0;\n1 -- 3: 0.631499 A, 0;\n1 -- 4: 1.07446 A, 180;\n2 -- 3: 0.0757193 A, 0;\n2 -- 4: 0.367239 A, 0;\n3 -- 4: 0.707219 A, 0;");
}

TEST(CalculationTest, DCCircuit2Test) {
    generate_test_file("dc_test.txt",
                       "define DCCircuit(A, B)\n A -- B, 3R;0C;0L; 15V, 0;\ndefine DCCircuit1(A, B, C, D)\n B -- C, 1R;0C;0L;\n B -- D, 10R;0C;0L;\n C -- D, 5R;0C;0L;\n C -- D, 10R;0C;0L;\n D -- A, 2R;0C;0L;");
    generate_calculation_test("include dc_test.txt\nDCCircuit(1, 2)\nDCCircuit1(1, 2, 3, 4)",
                              "1 -- 2: 1.86957 A, 0;\n2 -- 3: 1.30435 A, 0;\n2 -- 4: 0.565217 A, 0;\n3 -- 4: 0.869565 A, 0;\n3 -- 4: 0.434783 A, 0;\n4 -- 1: 1.86957 A, 0;");
}

TEST(CalculationTest, DCCircuit3Test) {
    generate_test_file("dc_test.txt",
                       "define DCCircuit(A, B)\n A -- B, 50R;0C;0L; 75V, 180;\n\ndefine DCCircuit1(A, B, C, D)\n B -- C, 50R;0C;0L;\n A -- C, 150R;0C;0L;\n A -- D, 100R;0C;0L; 100V, 180\n D -- C, 50R;0C;0L;");
    generate_calculation_test("include dc_test.txt\nDCCircuit(1, 2)\nDCCircuit1(1, 2, 3, 4)",
                              "1 -- 2: 0.142857 A, -180;\n1 -- 3: 0.404762 A, 7.01671e-15;\n1 -- 4: 0.261905 A, -180;\n2 -- 3: 0.142857 A, -180;\n4 -- 3: 0.261905 A, -180;");
}

TEST(CalculationTest, DCCircuit4Test) {
    generate_test_file("dc_test1.txt",
                       "define DCCircuit(A, B)\n A -- B, 50R;0C;0L; 75V, 180;");
    generate_test_file("dc_test2.txt",
                       "define DCCircuit1(A, B, C, D)\n B -- C, 50R;0C;0L;\n A -- C, 150R;0C;0L;\n A -- D, 100R;0C;0L; 100V, 0\n D -- C, 50R;0C;0L;");
    generate_calculation_test("include dc_test1.txt\ninclude dc_test2.txt\nDCCircuit(1, 2)\nDCCircuit1(1, 2, 3, 4)",
                              "1 -- 2: 0.714286 A, -180;\n1 -- 3: 0.0238095 A, 6.31504e-14;\n1 -- 4: 0.690476 A, 2.1776e-15;\n2 -- 3: 0.714286 A, -180;\n4 -- 3: 0.690476 A, 2.1776e-15;");
}

TEST(CalculationTest, DCCircuit5Test) {
    generate_test_file("dc_test1.txt",
                       "define Resistance0(A, B)\n A -- B, 0R;0C;0L;\n\n\n\n\n\n\ndefine Resistance1(A, B)\n A -- B, 1R;0C;0L;\ndefine Resistance2(A, B)\n A -- B, 2R;0C;0L;");
    generate_test_file("dc_test2.txt",
                       "include dc_test1.txt\n\ndefine Voltage(A, B)\n A -- B, 0R;0C;0L; 120V, 0");
    generate_calculation_test("include dc_test2.txt\n\nResistance1(1, 2)\nResistance0(1, 6)\nResistance2(2, 3)\nResistance2(2, 7)\nResistance2(3, 4)\nResistance2(3, 8)\nResistance1(4, 5)\nResistance2(4, 9)\nResistance0(5, 10)\n"
                              "Resistance1(6, 7)\nResistance0(6, 11)\nResistance2(7, 8)\nResistance2(7, 12)\nResistance2(8, 9)\nResistance2(8, 13)\nResistance1(9, 10)\nResistance2(9, 14)\nResistance0(10, 15)\n"
                              "Resistance1(11, 12)\nResistance2(12, 13)\nResistance2(13, 14)\nResistance1(14, 15)\nResistance0(11, 16)\nResistance0(15, 20)\n\nVoltage(20, 16)",
                              "1 -- 2: 20 A, 0;\n1 -- 6: 20 A, 180;\n2 -- 3: 20 A, 0;\n2 -- 7: 5.83329e-09 A, 180;\n3 -- 4: 20 A, 0;\n3 -- 8: 1.06581e-14 A, 180;\n4 -- 5: 20 A, 0;\n"
                              "4 -- 9: 5.83333e-09 A, 0;\n5 -- 10: 20 A, 0;\n6 -- 7: 20 A, 0;\n6 -- 11: 40.0001 A, 180;\n7 -- 8: 20 A, 0;\n7 -- 12: 9.16673e-09 A, 180;\n8 -- 9: 20 A, 0;\n"
                              "8 -- 13: 1.06581e-14 A, 180;\n9 -- 10: 20 A, 0;\n9 -- 14: 9.16665e-09 A, 0;\n10 -- 15: 40 A, 0;\n11 -- 12: 20 A, 0;\n11 -- 16: 60 A, 180;\n12 -- 13: 20 A, 0;\n"
                              "13 -- 14: 20 A, 0;\n14 -- 15: 20 A, 0;\n15 -- 20: 60 A, 0;\n20 -- 16: 60 A, 0;");
}

TEST(CalculationTest, ACCircuit1Test) {
    generate_test_file("ac_test.txt",
                       "\n\n\ndefine Loop(A)\n A -- A, 20R;106.1C;0.94L; 50V, 0\n\n\n");
    generate_calculation_test("\n\n\ninclude ac_test.txt\nLoop(1)\n\n\n\n\n",
                              "1 -- 1: 0.467093 A, 79.2317;");
}

TEST(CalculationTest, ACCircuit2Test) {
    generate_test_file("ac_test.txt",
                       "define R50L50V100(A, B)\n A -- B, 50R;0C;50L; 100V, 0\n");
    generate_calculation_test("define R100C50L100(A,B)\n A -- B, 100R;50C;0L;\n A -- B, 100R;0C;100L;\ninclude ac_test.txt\nR50L50V100(1, 2)\nR100C50L100(2, 1)",
                              "1 -- 2: 0.723241 A, -23.8387;\n2 -- 1: 0.496139 A, 7.12502;\n2 -- 1: 0.392232 A, -64.44;");
}

TEST(CalculationTest, ACCircuit3Test) {
    generate_test_file("ac_test.txt",
                       "define L10V141(A, B)\n A -- B, 0R;0C;10L; 141V, 0\n");
    generate_calculation_test("define R10C10(A,B)\n A -- B, 0R;10C;0L;\n A -- B, 10R;0C;0L;\ninclude ac_test.txt\nL10V141(1, 2)\nR10C10(2,1)",
                              "1 -- 2: 19.9404 A, -45;\n2 -- 1: 14.1 A, 0;\n2 -- 1: 14.1 A, -90;");
}

TEST(CalculationTest, ACCircuit4Test) {
    generate_test_file("ac_test.txt",
                       "define Loop()\n internal X\n X -- X, 12R;20C;36L; 100V, 0\nLoop()");
    generate_calculation_test("include ac_test.txt",
                              "1 -- 1: 5 A, -53.1301;");
}

TEST(CalculationTest, ACCircuit5Test) {
    generate_test_file("ac_test.txt",
                       "define R12V100(A,B)\n A -- B, 12R;0C;0L; 100V, 0\nR12V100(1,2)\n2 -- 3, 0R;20C;0L");
    generate_calculation_test("include ac_test.txt\n3 -- 1, 0R; 0C; 36L;",
                              "1 -- 2: 5 A, -53.1301;\n2 -- 3: 5 A, -53.1301;\n3 -- 1: 5 A, -53.1301;");
}

TEST(CalculationTest, ACCircuit6Test) {
    generate_test_file("ac_test.txt",
                       "define Loop()\n internal x\n x -- x, 1R;0C;0L; 1V, 0\nLoop()");
    generate_calculation_test("include ac_test.txt\n1 -- 2, 0R;0C;0L;\n2 -- 4, 0R;0C;0L;\n4 -- 1, 0R;0C;0L;",
                              "5 -- 5: 1 A, -0;\n1 -- 2: 0 A, 0;\n2 -- 4: 0 A, 0;\n4 -- 1: 0 A, 0;");
}

TEST(CalculationTest, TPCircuitTest) {
    generate_test_file("tp_test.txt",
                       "define ThreePhase(A,B,C,N)\n internal Z\n"
                       "    Z -- A, 0R; 0C; 0L; 220V, 0\n"
                       "    Z -- B, 0R; 0C; 0L; 220V, 120\n"
                       "    Z -- C, 0R; 0C; 0L; 220V, -120\n"
                       "    Z -- N, 0R; 0C; 0L;");
    generate_calculation_test("include tp_test.txt\nThreePhase(1,2,3,4)\ndefine Other(A,B,C,D)\n A -- D, 50R;0C;35L;\n B -- D, 50R;0C;0L;\n C -- D, 0R;0C;35L;\nOther(1, 2, 3, 4)",
                              "1 -- 4: 3.60462 A, -34.992;\n"
                              "2 -- 4: 4.4 A, -120;\n"
                              "3 -- 4: 6.28571 A, 30;\n"
                              "5 -- 1: 3.60464 A, -34.9918;\n"
                              "5 -- 2: 4.39999 A, -120;\n"
                              "5 -- 3: 6.2857 A, 30.0002;\n"
                              "5 -- 4: 6.28571 A, -150;");
}
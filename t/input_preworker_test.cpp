#include <gtest/gtest.h>
#include <fstream>

#include "../input_preworker.h"

TEST(ConstructTest, ConstructTest) {
    std::istringstream input;
    input_preworker preworker(input, "input", 0);
    EXPECT_EQ(preworker.get_output(), "");
}

TEST(TokenizerTest, EmptyTest) {
    std::istringstream input;
    input_preworker preworker(input, "input", 0);
    std::vector<std::vector<std::string>> expected_tokens;
    EXPECT_EQ(preworker.get_tokens(), expected_tokens);
}

TEST(TokenizerTest, OneEdgeTest) {
    std::istringstream input("1--2, 0R; 0C; 0L; 5V, -1;");
    input_preworker preworker(input, "input", 0);
    std::vector<std::vector<std::string>> expected_tokens;
    std::vector<std::string> expected_line;
    expected_line.emplace_back("1");
    expected_line.emplace_back("2");
    expected_line.emplace_back("0R");
    expected_line.emplace_back("0C");
    expected_line.emplace_back("0L");
    expected_line.emplace_back("5V");
    expected_line.emplace_back("-1");
    expected_tokens.push_back(expected_line);
    EXPECT_EQ(preworker.get_tokens(), expected_tokens);
}

TEST(TokenizerTest, DefineTest) {
    std::istringstream input("define Function(A,B,C)");
    input_preworker preworker(input, "input", 0);
    std::vector<std::vector<std::string>> expected_tokens;
    std::vector<std::string> expected_line;
    expected_line.emplace_back("define");
    expected_line.emplace_back("Function");
    expected_line.emplace_back("A");
    expected_line.emplace_back("B");
    expected_line.emplace_back("C");
    expected_tokens.push_back(expected_line);
    EXPECT_EQ(preworker.get_tokens(), expected_tokens);
}

TEST(TokenizerTest, DefineOneLineTest) {
    std::istringstream input("define Function(A,B,C)\n  internal Z");
    input_preworker preworker(input, "input", 0);
    std::vector<std::vector<std::string>> expected_tokens;
    std::vector<std::string> expected_line1;
    expected_line1.emplace_back("define");
    expected_line1.emplace_back("Function");
    expected_line1.emplace_back("A");
    expected_line1.emplace_back("B");
    expected_line1.emplace_back("C");
    expected_tokens.push_back(expected_line1);
    std::vector<std::string> expected_line2;
    expected_line2.emplace_back("");
    expected_line2.emplace_back("internal");
    expected_line2.emplace_back("Z");
    expected_tokens.push_back(expected_line2);
    EXPECT_EQ(preworker.get_tokens(), expected_tokens);
}

TEST(TokenizerTest, IncludeTest) {
    std::istringstream input("include ../test/test.txt");
    input_preworker preworker(input, "input", 0);
    std::vector<std::vector<std::string>> expected_tokens;
    std::vector<std::string> expected_line;
    expected_line.emplace_back("include");
    expected_line.emplace_back("../test/test.txt");
    expected_tokens.push_back(expected_line);
    EXPECT_EQ(preworker.get_tokens(), expected_tokens);
}

TEST(PreworkerTest, DefineTest) {
    std::istringstream input("define Function(A,B)\n A--B, 0R; 0C; 0L;\nFunction(1,2)");
    input_preworker preworker(input, "input", 0);
    preworker.perform_prework();
    std::string expected_output("1 2 0R 0C 0L \n");
    EXPECT_EQ(preworker.get_output(), expected_output);
}

TEST(PreworkerTest, EmptyTest) {
    std::istringstream input("");
    input_preworker preworker(input, "input", 0);
    preworker.perform_prework();
    std::string expected_output;
    EXPECT_EQ(preworker.get_output(), expected_output);
}

TEST(PreworkerTest, OnlyDefineTest) {
    std::istringstream input("define Function(A,B)\n A--B, 0R; 0C; 0L;");
    input_preworker preworker(input, "input", 0);
    preworker.perform_prework();
    std::string expected_output;
    EXPECT_EQ(preworker.get_output(), expected_output);
}

TEST(PreworkerTest, DefineEmptyLineTest) {
    std::istringstream input("define Function(A,B)\n A--B, 0R; 0C; 0L;\n B--A, 0R; 0C; 0L;\n\n1--2, 0R; 0C; 0L;");
    input_preworker preworker(input, "input", 0);
    preworker.perform_prework();
    std::string expected_output("1 2 0R 0C 0L \n");
    EXPECT_EQ(preworker.get_output(), expected_output);
}

TEST(PreworkerTest, EmptyLineBetweenEdgesTest) {
    std::istringstream input("1--2, 0R; 0C; 0L;\n\n2--1, 0R;0C;0L;");
    input_preworker preworker(input, "input", 0);
    preworker.perform_prework();
    std::string expected_output("1 2 0R 0C 0L \n2 1 0R 0C 0L \n");
    EXPECT_EQ(preworker.get_output(), expected_output);
}

TEST(PreworkerTest, IncludeTest) {
    std::ofstream file_to_include;
    file_to_include.open("test.txt");
    file_to_include << "define Function(A,B)\n A--B, 0R; 0C; 0L;";
    file_to_include.close();
    std::istringstream input("include test.txt\nFunction(1,2)");
    input_preworker preworker(input, "input", 0);
    preworker.perform_prework();
    std::string expected_output("1 2 0R 0C 0L \n");
    EXPECT_EQ(preworker.get_output(), expected_output);
}

TEST(PreworkerTest, IncludeAndCircuitTest) {
    std::ofstream file_to_include;
    file_to_include.open("test.txt");
    file_to_include << "define Function(A,B)\n A--B, 0R; 0C; 0L;\n B--A, 0R; 0C; 0L;\n1--2, 0R; 0C; 0L;";
    file_to_include.close();
    std::istringstream input("include test.txt\nFunction(3,4)");
    input_preworker preworker(input, "input", 0);
    preworker.perform_prework();
    std::string expected_output("1 2 0R 0C 0L \n3 4 0R 0C 0L \n4 3 0R 0C 0L \n");
    EXPECT_EQ(preworker.get_output(), expected_output);
}

TEST(PreworkerTest, InternalTest) {
    std::ofstream file_to_include;
    file_to_include.open("test.txt");
    file_to_include << "define Function(A,B)\n internal Z\n A--Z, 0R; 0C; 0L;\n B--Z, 0R; 0C; 0L;";
    file_to_include.close();
    std::istringstream input("include test.txt\nFunction(1,2)");
    input_preworker preworker(input, "input", 0);
    preworker.perform_prework();
    std::string expected_output("1 3 0R 0C 0L \n2 3 0R 0C 0L \n");
    EXPECT_EQ(preworker.get_output(), expected_output);
}

TEST(PreworkerTest, NoParamsFunctionWithInternalTest) {
    std::ofstream file_to_include;
    file_to_include.open("test.txt");
    file_to_include << "define Function()\n internal Z\n Z--Z, 0R; 0C; 0L;";
    file_to_include.close();
    std::istringstream input("include test.txt\nFunction()");
    input_preworker preworker(input, "input", 0);
    preworker.perform_prework();
    std::string expected_output("1 1 0R 0C 0L \n");
    EXPECT_EQ(preworker.get_output(), expected_output);
}

TEST(PreworkerTest, NoParamsFunctionTest) {
    std::ofstream file_to_include;
    file_to_include.open("test.txt");
    file_to_include << "define Function()";
    file_to_include.close();
    std::istringstream input("include test.txt\nFunction()");
    input_preworker preworker(input, "input", 0);
    preworker.perform_prework();
    std::string expected_output;
    EXPECT_EQ(preworker.get_output(), expected_output);
}

TEST(PreworkerTest, EmptyIncludeTest) {
    std::ofstream file_to_include;
    file_to_include.open("test.txt");
    file_to_include.close();
    std::istringstream input("include test.txt\n1 -- 2, 0R;0C;0L;");
    input_preworker preworker(input, "input", 0);
    preworker.perform_prework();
    std::string expected_output("1 2 0R 0C 0L \n");
    EXPECT_EQ(preworker.get_output(), expected_output);
}

TEST(PreworkerTest, UnknownElementTest) {
    std::ofstream file_to_include;
    file_to_include.open("test.txt");
    file_to_include << "define Function()";
    file_to_include.close();
    std::istringstream input("include test.txt\nFunc()");
    input_preworker preworker(input, "input", 0);
    EXPECT_THROW(preworker.perform_prework(), input_preworker::UnknownElementException);
}

TEST(PreworkerTest, InvalidParameterNumberMoreTest) {
    std::ofstream file_to_include;
    file_to_include.open("test.txt");
    file_to_include << "define Function(A, B)";
    file_to_include.close();
    std::istringstream input("include test.txt\nFunction(1, 2, 3)");
    input_preworker preworker(input, "input", 0);
    EXPECT_THROW(preworker.perform_prework(), input_preworker::InvalidParameterNumberException);
}

TEST(PreworkerTest, InvalidParameterNumberLessTest) {
    std::ofstream file_to_include;
    file_to_include.open("test.txt");
    file_to_include << "define Function(A, B)";
    file_to_include.close();
    std::istringstream input("include test.txt\nFunction(1)");
    input_preworker preworker(input, "input", 0);
    EXPECT_THROW(preworker.perform_prework(), input_preworker::InvalidParameterNumberException);
}

TEST(PreworkerTest, UnexpectedTokenTest) {
    std::ofstream file_to_include;
    file_to_include.open("test.txt");
    file_to_include << "define Function_(A, B)";
    file_to_include.close();
    std::istringstream input("include test.txt\nFunction(1, 2, 3)");
    input_preworker preworker(input, "input", 0);
    EXPECT_THROW(preworker.perform_prework(), input_preworker::UnexpectedTokenException);
}

TEST(PreworkerTest, EmptyLineInsideDefineTest) {
    std::ofstream file_to_include;
    file_to_include.open("test.txt");
    file_to_include << "define Function(A,B)\n A--B, 0R; 0C; 0L;\n\n B--A, 0R; 0C; 0L;\n\n";
    file_to_include.close();
    std::istringstream input("include test.txt\nFunction(3,4)");
    input_preworker preworker(input, "input", 0);
    preworker.perform_prework();
    std::string expected_output("3 4 0R 0C 0L \n4 3 0R 0C 0L \n");
    EXPECT_EQ(preworker.get_output(), expected_output);
}

TEST(InvalidInputTest, BrokenDefineLine1Test) {
    std::istringstream input("define function(A, B, )");
    EXPECT_THROW(input_preworker(input, "input", 0), input_preworker::UnexpectedTokenException);
}

TEST(InvalidInputTest, BrokenDefineLine2Test) {
    std::istringstream input("define function(A, B) C");
    EXPECT_THROW(input_preworker(input, "input", 0), input_preworker::UnexpectedTokenException);
}

TEST(InvalidInputTest, BrokenDefineLine3Test) {
    std::istringstream input("define function(A, B");
    EXPECT_THROW(input_preworker(input, "input", 0), input_preworker::UnexpectedTokenException);
}

TEST(InvalidInputTest, BrokenDefineLine4Test) {
    std::istringstream input("define (A, B)");
    EXPECT_THROW(input_preworker(input, "input", 0), input_preworker::UnexpectedTokenException);
}

TEST(InvalidInputTest, BrokenDefineLine5Test) {
    std::istringstream input("define A, B)");
    EXPECT_THROW(input_preworker(input, "input", 0), input_preworker::UnexpectedTokenException);
}

TEST(InvalidInputTest, BrokenDefineLine6Test) {
    std::istringstream input("define ,)");
    EXPECT_THROW(input_preworker(input, "input", 0), input_preworker::UnexpectedTokenException);
}

TEST(InvalidInputTest, BrokenDefineLine7Test) {
    std::istringstream input("define");
    EXPECT_THROW(input_preworker(input, "input", 0), input_preworker::UnexpectedTokenException);
}

TEST(InvalidInputTest, BrokenDefineLine8Test) {
    std::istringstream input("define f(A B)");
    EXPECT_THROW(input_preworker(input, "input", 0), input_preworker::UnexpectedTokenException);
}

TEST(InvalidInputTest, BrokenElementCall1Test) {
    std::istringstream input("define function(A, B)\nfunction(1, 2,)");
    EXPECT_THROW(input_preworker(input, "input", 0), input_preworker::UnexpectedTokenException);
}

TEST(InvalidInputTest, BrokenElementCall2Test) {
    std::istringstream input("define function(A, B)\nfunction(1, 2");
    EXPECT_THROW(input_preworker(input, "input", 0), input_preworker::UnexpectedTokenException);
}

TEST(InvalidInputTest, BrokenElementCall3Test) {
    std::istringstream input("define function(A, B)\nfunction1, 2)");
    EXPECT_THROW(input_preworker(input, "input", 0), input_preworker::UnexpectedTokenException);
}

TEST(InvalidInputTest, BrokenElementCall4Test) {
    std::istringstream input("define function(A, B)\nfunction 1, 2)");
    EXPECT_THROW(input_preworker(input, "input", 0), input_preworker::UnexpectedTokenException);
}

TEST(InvalidInputTest, BrokenElementCall5Test) {
    std::istringstream input("define function(A, B)\nfunction");
    EXPECT_THROW(input_preworker(input, "input", 0), input_preworker::UnexpectedTokenException);
}

TEST(InvalidInputTest, BrokenElementCall6Test) {
    std::istringstream input("define function(A, B)\nfunction(1 2)");
    EXPECT_THROW(input_preworker(input, "input", 0), input_preworker::UnexpectedTokenException);
}

TEST(InvalidInputTest, MultipleDefinition1Test) {
    std::istringstream input("define function(A, B)\ndefine function(A, B)\nfunction(1, 2)");
    input_preworker preworker(input, "input", 0);
    EXPECT_THROW(preworker.perform_prework(), input_preworker::MultipleDefinitionException);
}

TEST(InvalidInputTest, MultipleDefinition2Test) {
    std::ofstream file_to_include;
    file_to_include.open("test.txt");
    file_to_include << "define Function(A, B)";
    file_to_include.close();
    std::istringstream input("define Function(A, B)\ninclude test.txt\nFunction(1, 2)");
    input_preworker preworker(input, "input", 0);
    EXPECT_THROW(preworker.perform_prework(), input_preworker::MultipleDefinitionException);
}

TEST(InvalidInputTest, UndefinedParameterTest) {
    std::istringstream input("define function(A, B)\n A -- C, 0R; 0C; 0L;");
    input_preworker preworker(input, "input", 0);
    EXPECT_THROW(preworker.perform_prework(), input_preworker::UndefinedVariableException);
}

TEST(InvalidInputTest, InvalidElementEdge1Test) {
    std::istringstream input("define function(A, B)\n A - B, 0R; 0C; 0L;");
    EXPECT_THROW(input_preworker(input, "input", 0), input_preworker::UnexpectedTokenException);
}

TEST(InvalidInputTest, InvalidElementEdge2Test) {
    std::istringstream input("define function(A, B)\n A -- B 0R; 0C; 0L;");
    EXPECT_THROW(input_preworker(input, "input", 0), input_preworker::UnexpectedTokenException);
}

TEST(InvalidInputTest, InvalidElementEdge3Test) {
    std::istringstream input("define function(A, B)\n A -- B, 0; 0C; 0L;");
    EXPECT_THROW(input_preworker(input, "input", 0), input_preworker::UnexpectedTokenException);
}

TEST(InvalidInputTest, InvalidElementEdge4Test) {
    std::istringstream input("define function(A, B)\n A -- B, 0; 0C; 0L;");
    EXPECT_THROW(input_preworker(input, "input", 0), input_preworker::UnexpectedTokenException);
}

TEST(InvalidInputTest, InvalidElementEdge5Test) {
    std::istringstream input("define function(A, B)\n A -- B, 0r; 0C; 0L;");
    EXPECT_THROW(input_preworker(input, "input", 0), input_preworker::UnexpectedTokenException);
}

TEST(InvalidInputTest, InvalidElementEdge6Test) {
    std::istringstream input("define function(A, B)\n A -- B, 0R 0C; 0L;");
    EXPECT_THROW(input_preworker(input, "input", 0), input_preworker::UnexpectedTokenException);
}

TEST(InvalidInputTest, InvalidElementEdge7Test) {
    std::istringstream input("define function(A, B)\n A -- B, 0R; C; 0L;");
    EXPECT_THROW(input_preworker(input, "input", 0), input_preworker::UnexpectedTokenException);
}

TEST(InvalidInputTest, InvalidElementEdge8Test) {
    std::istringstream input("define function(A, B)\n A -- B, 0R; 0..C; 0L;");
    EXPECT_THROW(input_preworker(input, "input", 0), input_preworker::UnexpectedTokenException);
}

TEST(InvalidInputTest, InvalidElementEdge9Test) {
    std::istringstream input("define function(A, B)\n A -- B, 0R; 0.C 0L;");
    EXPECT_THROW(input_preworker(input, "input", 0), input_preworker::UnexpectedTokenException);
}

TEST(InvalidInputTest, InvalidElementEdge10Test) {
    std::istringstream input("define function(A, B)\n A -- B, 0R; 0.C; 0..L;");
    EXPECT_THROW(input_preworker(input, "input", 0), input_preworker::UnexpectedTokenException);
}

TEST(InvalidInputTest, InvalidElementEdge11Test) {
    std::istringstream input("define function(A, B)\n A -- B, 0R; 0.C; 0.l;");
    EXPECT_THROW(input_preworker(input, "input", 0), input_preworker::UnexpectedTokenException);
}

TEST(InvalidInputTest, InvalidElementEdge12Test) {
    std::istringstream input("define function(A, B)\n A -- B, 0R; 0.C; 0.L 12V, 0");
    EXPECT_THROW(input_preworker(input, "input", 0), input_preworker::UnexpectedTokenException);
}

TEST(InvalidInputTest, InvalidElementEdge13Test) {
    std::istringstream input("define function(A, B)\n A -- B, 0R; 0.C; 0.L; 12v, 0");
    EXPECT_THROW(input_preworker(input, "input", 0), input_preworker::UnexpectedTokenException);
}

TEST(InvalidInputTest, InvalidElementEdge14Test) {
    std::istringstream input("define function(A, B)\n A -- B, 0R; 0.C; 0.L; 12V 0;");
    EXPECT_THROW(input_preworker(input, "input", 0), input_preworker::UnexpectedTokenException);
}

TEST(InvalidInputTest, InvalidElementEdge15Test) {
    std::istringstream input("define function(A, B)\n A -- B, 0R; 0.C; 0.L; 12V, 0Ph");
    EXPECT_THROW(input_preworker(input, "input", 0), input_preworker::UnexpectedTokenException);
}

TEST(InvalidInputTest, InvalidElementEdge16Test) {
    std::istringstream input("define function(A, B)\n C -- B, 0R; 0.C; 0.L; 12V, 0");
    input_preworker preworker(input, "input", 0);
    EXPECT_THROW(preworker.perform_prework(), input_preworker::UndefinedVariableException);
}

TEST(InvalidInputTest, InvalidElementEdge17Test) {
    std::istringstream input("define function(A, B)\n A -- B, 0R; 0.C; 0.L; 12V");
    EXPECT_THROW(input_preworker(input, "input", 0), input_preworker::UnexpectedTokenException);
}

TEST(InvalidInputTest, InvalidElementEdge18Test) {
    std::istringstream input("define function(A, B)\n A -- B, 0R; 0.C; 0.L; 12V, 0;;");
    EXPECT_THROW(input_preworker(input, "input", 0), input_preworker::UnexpectedTokenException);
}

TEST(InvalidInputTest, InvalidElementEdge19Test) {
    std::istringstream input("define function(A, B)\n A -- B, ");
    EXPECT_THROW(input_preworker(input, "input", 0), input_preworker::UnexpectedTokenException);
}

TEST(InvalidInputTest, InvalidEdge1Test) {
    std::istringstream input("1 - 2, 0R; 0C; 0L;");
    EXPECT_THROW(input_preworker(input, "input", 0), input_preworker::UnexpectedTokenException);
}

TEST(InvalidInputTest, InvalidEdge2Test) {
    std::istringstream input("1 -- 2 0R; 0C; 0L;");
    EXPECT_THROW(input_preworker(input, "input", 0), input_preworker::UnexpectedTokenException);
}

TEST(InvalidInputTest, InvalidEdge3Test) {
    std::istringstream input("1 -- 2, 0; 0C; 0L;");
    EXPECT_THROW(input_preworker(input, "input", 0), input_preworker::UnexpectedTokenException);
}

TEST(InvalidInputTest, InvalidEdge4Test) {
    std::istringstream input("1 -- 2, 0; 0C; 0L;");
    EXPECT_THROW(input_preworker(input, "input", 0), input_preworker::UnexpectedTokenException);
}

TEST(InvalidInputTest, InvalidEdge5Test) {
    std::istringstream input("1 -- 2, 0r; 0C; 0L;");
    EXPECT_THROW(input_preworker(input, "input", 0), input_preworker::UnexpectedTokenException);
}

TEST(InvalidInputTest, InvalidEdge6Test) {
    std::istringstream input("1 -- 2, 0R 0C; 0L;");
    EXPECT_THROW(input_preworker(input, "input", 0), input_preworker::UnexpectedTokenException);
}

TEST(InvalidInputTest, InvalidEdge7Test) {
    std::istringstream input("1 -- 2, 0R; C; 0L;");
    EXPECT_THROW(input_preworker(input, "input", 0), input_preworker::UnexpectedTokenException);
}

TEST(InvalidInputTest, InvalidEdge8Test) {
    std::istringstream input("1 -- 2, 0R; 0..C; 0L;");
    EXPECT_THROW(input_preworker(input, "input", 0), input_preworker::UnexpectedTokenException);
}

TEST(InvalidInputTest, InvalidEdge9Test) {
    std::istringstream input("1 -- 2, 0R; 0.C 0L;");
    EXPECT_THROW(input_preworker(input, "input", 0), input_preworker::UnexpectedTokenException);
}

TEST(InvalidInputTest, InvalidEdge10Test) {
    std::istringstream input("1 -- 2, 0R; 0.C; 0..L;");
    EXPECT_THROW(input_preworker(input, "input", 0), input_preworker::UnexpectedTokenException);
}

TEST(InvalidInputTest, InvalidEdge11Test) {
    std::istringstream input("1 -- 2, 0R; 0.C; 0.l;");
    EXPECT_THROW(input_preworker(input, "input", 0), input_preworker::UnexpectedTokenException);
}

TEST(InvalidInputTest, InvalidEdge12Test) {
    std::istringstream input("1 -- 2, 0R; 0.C; 0.L 12V, 0");
    EXPECT_THROW(input_preworker(input, "input", 0), input_preworker::UnexpectedTokenException);
}

TEST(InvalidInputTest, InvalidEdge13Test) {
    std::istringstream input("1 -- 2, 0R; 0.C; 0.L; 12v, 0");
    EXPECT_THROW(input_preworker(input, "input", 0), input_preworker::UnexpectedTokenException);
}

TEST(InvalidInputTest, InvalidEdge14Test) {
    std::istringstream input("1 -- 2, 0R; 0.C; 0.L; 12V 0;");
    EXPECT_THROW(input_preworker(input, "input", 0), input_preworker::UnexpectedTokenException);
}

TEST(InvalidInputTest, InvalidEdge15Test) {
    std::istringstream input("1 -- 2, 0R; 0.C; 0.L; 12V, 0Ph");
    EXPECT_THROW(input_preworker(input, "input", 0), input_preworker::UnexpectedTokenException);
}

TEST(InvalidInputTest, InvalidEdge16Test) {
    std::istringstream input("1 -- 2., 0R; 0.C; 0.L; 12V, 0");
    EXPECT_THROW(input_preworker(input, "input", 0), input_preworker::UnexpectedTokenException);
}

TEST(InvalidInputTest, InvalidEdge17Test) {
    std::istringstream input("1 -- 2, 0R; 0.C; 0.L; 12V");
    EXPECT_THROW(input_preworker(input, "input", 0), input_preworker::UnexpectedTokenException);
}

TEST(InvalidInputTest, InvalidEdge18Test) {
    std::istringstream input("1 -- 2, 0R; 0.C; 0.L; 12V, 0;;");
    EXPECT_THROW(input_preworker(input, "input", 0), input_preworker::UnexpectedTokenException);
}

TEST(InvalidInputTest, InvalidEdge19Test) {
    std::istringstream input("1 -- 2, ");
    EXPECT_THROW(input_preworker(input, "input", 0), input_preworker::UnexpectedTokenException);
}

TEST(InvalidInputTest, BrokenIncludeTest) {
    std::istringstream input("include");
    EXPECT_THROW(input_preworker preworker(input, "input", 0), input_preworker::UnexpectedTokenException);
}

TEST(InvalidInputTest, BrokenInclude1Test) {
    std::istringstream input("include 1");
    input_preworker preworker(input, "input", 0);
    EXPECT_THROW(preworker.perform_prework(), input_preworker::FileOpenException);
}

TEST(InvalidInputTest, BrokenInclude2Test) {
    std::istringstream input("include a a");
    EXPECT_THROW(input_preworker(input, "input", 0), input_preworker::UnexpectedTokenException);
}

TEST(FilePathTest, FilePathTest) {
    std::istringstream input("include a a");
    EXPECT_THROW(input_preworker(input, "../test.txt", 0), input_preworker::UnexpectedTokenException);
}

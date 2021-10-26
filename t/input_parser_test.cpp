#include <gtest/gtest.h>

#include "../input_parser.h"

TEST(ParseTest, SingleIterationNoEMFTest) {
    input_parser parser("1 -- 2, 0.1; 1 -- 3, 0.2;");
    parser.make_iteration();
    auto [incoming_vertex, outcoming_vertex, resistance, emf] = parser.get_current_edge_info();
    EXPECT_EQ(1, outcoming_vertex);
    EXPECT_EQ(2, incoming_vertex);
    EXPECT_DOUBLE_EQ(0.1, resistance);
    EXPECT_FALSE(parser.is_emf_included());
    EXPECT_FALSE(parser.is_eof());
}

TEST(ParseTest, SingleIterationWithEMFTest) {
    input_parser parser("1 -- 2, 0.1; 0.5V; 1 -- 3, 0.2;");
    parser.make_iteration();
    auto [incoming_vertex, outcoming_vertex, resistance, emf] = parser.get_current_edge_info();
    EXPECT_EQ(1, outcoming_vertex);
    EXPECT_EQ(2, incoming_vertex);
    EXPECT_DOUBLE_EQ(0.1, resistance);
    EXPECT_TRUE(parser.is_emf_included());
    EXPECT_DOUBLE_EQ(0.5, emf);
    EXPECT_FALSE(parser.is_eof());
}

TEST(ParseTest, TwoIterationWithEMFFirstNoEMFSecondTest) {
    input_parser parser("1 -- 2, 0.1; 0.5V; 1 -- 3, 0.2;");
    parser.make_iteration();
    parser.make_iteration();
    auto [incoming_vertex, outcoming_vertex, resistance, emf] = parser.get_current_edge_info();
    EXPECT_EQ(1, outcoming_vertex);
    EXPECT_EQ(3, incoming_vertex);
    EXPECT_DOUBLE_EQ(0.2, resistance);
    EXPECT_FALSE(parser.is_emf_included());
    EXPECT_TRUE(parser.is_eof());
}

TEST(ParseTest, TwoIterationNoEMFFirstWithEMFSecondTest) {
    input_parser parser("1 -- 2, 0.1; 1 -- 3, 0.2; 0.6V;");
    parser.make_iteration();
    parser.make_iteration();
    auto [incoming_vertex, outcoming_vertex, resistance, emf] = parser.get_current_edge_info();
    EXPECT_EQ(1, outcoming_vertex);
    EXPECT_EQ(3, incoming_vertex);
    EXPECT_DOUBLE_EQ(0.2, resistance);
    EXPECT_TRUE(parser.is_emf_included());
    EXPECT_EQ(0.6, emf);
    EXPECT_TRUE(parser.is_eof());
}

TEST(ParseTest, TwoIterationWithEMFTest) {
    input_parser parser("1 -- 2, 0.1; 0.5V; 1 -- 3, 0.2; 0.6V;");
    parser.make_iteration();
    parser.make_iteration();
    auto [incoming_vertex, outcoming_vertex, resistance, emf] = parser.get_current_edge_info();
    EXPECT_EQ(1, outcoming_vertex);
    EXPECT_EQ(3, incoming_vertex);
    EXPECT_DOUBLE_EQ(0.2, resistance);
    EXPECT_TRUE(parser.is_emf_included());
    EXPECT_EQ(0.6, emf);
    EXPECT_TRUE(parser.is_eof());
}

TEST(ParseTest, SingleDashBetweenVerticesTest) {
    input_parser parser("1 - 2, 0.1;");
    parser.make_iteration();
    auto [incoming_vertex, outcoming_vertex, resistance, emf] = parser.get_current_edge_info();
    EXPECT_EQ(1, outcoming_vertex);
    EXPECT_EQ(2, incoming_vertex);
    EXPECT_DOUBLE_EQ(0.1, resistance);
    EXPECT_FALSE(parser.is_emf_included());
    EXPECT_TRUE(parser.is_eof());
}

TEST(ParseTest, NoDashBetweenVerticesTest) {
    input_parser parser("1 2, 0.1;");
    parser.make_iteration();
    auto [incoming_vertex, outcoming_vertex, resistance, emf] = parser.get_current_edge_info();
    EXPECT_EQ(1, outcoming_vertex);
    EXPECT_EQ(2, incoming_vertex);
    EXPECT_DOUBLE_EQ(0.1, resistance);
    EXPECT_FALSE(parser.is_emf_included());
    EXPECT_TRUE(parser.is_eof());
}

TEST(ParseTest, NoCommaBeforeResistanceTest) {
    input_parser parser("1 -- 2 0.1;");
    parser.make_iteration();
    auto [incoming_vertex, outcoming_vertex, resistance, emf] = parser.get_current_edge_info();
    EXPECT_EQ(1, outcoming_vertex);
    EXPECT_EQ(2, incoming_vertex);
    EXPECT_DOUBLE_EQ(0.1, resistance);
    EXPECT_FALSE(parser.is_emf_included());
    EXPECT_TRUE(parser.is_eof());
}

TEST(ParseTest, NoSemicolonAfterResistanceTest) {
    input_parser parser("1 -- 2, 0.1");
    parser.make_iteration();
    auto [incoming_vertex, outcoming_vertex, resistance, emf] = parser.get_current_edge_info();
    EXPECT_EQ(1, outcoming_vertex);
    EXPECT_EQ(2, incoming_vertex);
    EXPECT_DOUBLE_EQ(0.1, resistance);
    EXPECT_FALSE(parser.is_emf_included());
    EXPECT_TRUE(parser.is_eof());
}

TEST(ParseTest, NoSemicolonAfterEMFTest) {
    input_parser parser("1 -- 2, 0.1; 0.5V");
    parser.make_iteration();
    auto [incoming_vertex, outcoming_vertex, resistance, emf] = parser.get_current_edge_info();
    EXPECT_EQ(1, outcoming_vertex);
    EXPECT_EQ(2, incoming_vertex);
    EXPECT_DOUBLE_EQ(0.1, resistance);
    EXPECT_TRUE(parser.is_emf_included());
    EXPECT_DOUBLE_EQ(0.5, emf);
    EXPECT_TRUE(parser.is_eof());
}

TEST(ParseTest, NoPunctuationTest) {
    input_parser parser("1 2 0.1 0.5V");
    parser.make_iteration();
    auto [incoming_vertex, outcoming_vertex, resistance, emf] = parser.get_current_edge_info();
    EXPECT_EQ(1, outcoming_vertex);
    EXPECT_EQ(2, incoming_vertex);
    EXPECT_DOUBLE_EQ(0.1, resistance);
    EXPECT_TRUE(parser.is_emf_included());
    EXPECT_DOUBLE_EQ(0.5, emf);
    EXPECT_TRUE(parser.is_eof());
}

TEST(ParserThrowsInvalidInputExceptionTest, UnexpectedCharacterBeforeFirstVertexTest) {
    input_parser parser("a1 -- 2, 0.1; 0.5V;");
    EXPECT_THROW(parser.make_iteration(), input_parser::InvalidInputException);
}

TEST(ParserThrowsInvalidInputExceptionTest, UnexpectedCharacterBeforeFirstDashTest) {
    input_parser parser("1 a-- 2, 0.1; 0.5V;");
    EXPECT_THROW(parser.make_iteration(), input_parser::InvalidInputException);
}

TEST(ParserThrowsInvalidInputExceptionTest, UnexpectedCharacterBeforeSecondDashTest) {
    input_parser parser("1 -a- 2, 0.1; 0.5V;");
    EXPECT_THROW(parser.make_iteration(), input_parser::InvalidInputException);
}

TEST(ParserThrowsInvalidInputExceptionTest, UnexpectedCharacterBeforeSecondVertexTest) {
    input_parser parser("1 -- a2, 0.1; 0.5V;");
    EXPECT_THROW(parser.make_iteration(), input_parser::InvalidInputException);
}

TEST(ParserThrowsInvalidInputExceptionTest, UnexpectedCharacterBeforeCommaTest) {
    input_parser parser("1 -- 2a, 0.1; 0.5V;");
    EXPECT_THROW(parser.make_iteration(), input_parser::InvalidInputException);
}

TEST(ParserThrowsInvalidInputExceptionTest, UnexpectedCharacterBeforeResistanceTest) {
    input_parser parser("1 -- 2, a0.1; 0.5V;");
    EXPECT_THROW(parser.make_iteration(), input_parser::InvalidInputException);
}

TEST(ParserThrowsInvalidInputExceptionTest, UnexpectedCharacterBeforeFirstSemicolonTest) {
    input_parser parser("1 -- 2, 0.1a; 0.5V;");
    EXPECT_THROW(parser.make_iteration(), input_parser::InvalidInputException);
}

TEST(ParserThrowsInvalidInputExceptionTest, UnexpectedCharacterBeforeEMFTest) {
    input_parser parser("1 -- 2, 0.1; a0.5V;");
    EXPECT_THROW(parser.make_iteration(), input_parser::InvalidInputException);
}

TEST(ParserThrowsInvalidInputExceptionTest, UnexpectedCharacterAfterEMFTest) {
    input_parser parser("1 -- 2, 0.1; 0.5aV;");
    parser.make_iteration();
    EXPECT_THROW(parser.make_iteration(), input_parser::InvalidInputException);
}

TEST(ParserThrowsInvalidInputExceptionTest, UnexpectedCharacterBeforeSecondSemicolonTest) {
    input_parser parser("1 -- 2, 0.1; 0.5Va;");
    parser.make_iteration();
    EXPECT_THROW(parser.make_iteration(), input_parser::InvalidInputException);
}

TEST(ParserThrowsInvalidInputExceptionTest, OnlyDashInputTest) {
    input_parser parser("-");
    EXPECT_THROW(parser.make_iteration(), input_parser::InvalidInputException);
}

TEST(ParserThrowsInvalidInputExceptionTest, DashBeforeVoltageInputTest) {
    input_parser parser("1 -- 2, 4.0; - 2.0V; 2 -- 1, 4.0;");
    EXPECT_THROW(parser.make_iteration(), input_parser::InvalidInputException);
}

TEST(ParserThrowsInvalidInputExceptionTest, DashBeforeResistanceInputTest) {
    input_parser parser("1 -- 2, - 4.0; 2.0V;");
    EXPECT_THROW(parser.make_iteration(), input_parser::InvalidInputException);
}

TEST(ParserThrowsInvalidInputExceptionTest, DashEOFTest) {
    input_parser parser("1 -- 2, 4.0; 2.0V;-");
    parser.make_iteration();
    EXPECT_THROW(parser.make_iteration(), input_parser::InvalidInputException);
}
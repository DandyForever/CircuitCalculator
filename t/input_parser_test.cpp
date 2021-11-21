#include <gtest/gtest.h>

#include "../input_parser.h"
#include "../complex_number.h"

TEST(DcInputTest, SingleIterationNoEMFTest) {
    input_parser parser("1 -- 2, 0.1; 1 -- 3, 0.2;");
    parser.make_iteration();
    auto [incoming_vertex, outcoming_vertex, resistance, emf] = parser.get_current_edge_info();
    EXPECT_EQ(1, outcoming_vertex);
    EXPECT_EQ(2, incoming_vertex);
    EXPECT_EQ(std::complex<double>(0.1), resistance);
    EXPECT_FALSE(parser.is_emf_included());
    EXPECT_FALSE(parser.is_eof());
}

TEST(DcInputTest, SingleIterationWithEMFTest) {
    input_parser parser("1 -- 2, 0.1; 0.5V; 1 -- 3, 0.2;");
    parser.make_iteration();
    auto [incoming_vertex, outcoming_vertex, resistance, emf] = parser.get_current_edge_info();
    EXPECT_EQ(1, outcoming_vertex);
    EXPECT_EQ(2, incoming_vertex);
    std::complex<double>(0.1);
    EXPECT_EQ(std::complex<double>(0.1), resistance);
    EXPECT_TRUE(parser.is_emf_included());
    EXPECT_EQ(0.5, emf);
    EXPECT_FALSE(parser.is_eof());
}

TEST(DcInputTest, TwoIterationWithEMFFirstNoEMFSecondTest) {
    input_parser parser("1 -- 2, 0.1; 0.5V; 1 -- 3, 0.2;");
    parser.make_iteration();
    parser.make_iteration();
    auto [incoming_vertex, outcoming_vertex, resistance, emf] = parser.get_current_edge_info();
    EXPECT_EQ(1, outcoming_vertex);
    EXPECT_EQ(3, incoming_vertex);
    EXPECT_EQ(std::complex<double>(0.2), resistance);
    EXPECT_FALSE(parser.is_emf_included());
    EXPECT_TRUE(parser.is_eof());
}

TEST(DcInputTest, TwoIterationNoEMFFirstWithEMFSecondTest) {
    input_parser parser("1 -- 2, 0.1; 1 -- 3, 0.2; 0.6V;");
    parser.make_iteration();
    parser.make_iteration();
    auto [incoming_vertex, outcoming_vertex, resistance, emf] = parser.get_current_edge_info();
    EXPECT_EQ(1, outcoming_vertex);
    EXPECT_EQ(3, incoming_vertex);
    EXPECT_EQ(std::complex<double>(0.2), resistance);
    EXPECT_TRUE(parser.is_emf_included());
    EXPECT_EQ(0.6, emf);
    EXPECT_TRUE(parser.is_eof());
}

TEST(DcInputTest, TwoIterationWithEMFTest) {
    input_parser parser("1 -- 2, 0.1; 0.5V; 1 -- 3, 0.2; 0.6V;");
    parser.make_iteration();
    parser.make_iteration();
    auto [incoming_vertex, outcoming_vertex, resistance, emf] = parser.get_current_edge_info();
    EXPECT_EQ(1, outcoming_vertex);
    EXPECT_EQ(3, incoming_vertex);
    EXPECT_EQ(std::complex<double>(0.2), resistance);
    EXPECT_TRUE(parser.is_emf_included());
    EXPECT_EQ(0.6, emf);
    EXPECT_TRUE(parser.is_eof());
}

TEST(SkipSyntaxTest, SingleDashBetweenVerticesTest) {
    input_parser parser("1 - 2, 0.1;");
    parser.make_iteration();
    auto [incoming_vertex, outcoming_vertex, resistance, emf] = parser.get_current_edge_info();
    EXPECT_EQ(1, outcoming_vertex);
    EXPECT_EQ(2, incoming_vertex);
    EXPECT_EQ(std::complex<double>(0.1), resistance);
    EXPECT_FALSE(parser.is_emf_included());
    EXPECT_TRUE(parser.is_eof());
}

TEST(SkipSyntaxTest, NoDashBetweenVerticesTest) {
    input_parser parser("1 2, 0.1;");
    parser.make_iteration();
    auto [incoming_vertex, outcoming_vertex, resistance, emf] = parser.get_current_edge_info();
    EXPECT_EQ(1, outcoming_vertex);
    EXPECT_EQ(2, incoming_vertex);
    EXPECT_EQ(std::complex<double>(0.1), resistance);
    EXPECT_FALSE(parser.is_emf_included());
    EXPECT_TRUE(parser.is_eof());
}

TEST(SkipSyntaxTest, NoCommaBeforeResistanceTest) {
    input_parser parser("1 -- 2 0.1;");
    parser.make_iteration();
    auto [incoming_vertex, outcoming_vertex, resistance, emf] = parser.get_current_edge_info();
    EXPECT_EQ(1, outcoming_vertex);
    EXPECT_EQ(2, incoming_vertex);
    EXPECT_EQ(std::complex<double>(0.1), resistance);
    EXPECT_FALSE(parser.is_emf_included());
    EXPECT_TRUE(parser.is_eof());
}

TEST(SkipSyntaxTest, NoSemicolonAfterResistanceTest) {
    input_parser parser("1 -- 2, 0.1");
    parser.make_iteration();
    auto [incoming_vertex, outcoming_vertex, resistance, emf] = parser.get_current_edge_info();
    EXPECT_EQ(1, outcoming_vertex);
    EXPECT_EQ(2, incoming_vertex);
    EXPECT_EQ(std::complex<double>(0.1), resistance);
    EXPECT_FALSE(parser.is_emf_included());
    EXPECT_TRUE(parser.is_eof());
}

TEST(SkipSyntaxTest, NoSemicolonAfterEMFTest) {
    input_parser parser("1 -- 2, 0.1; 0.5V");
    parser.make_iteration();
    auto [incoming_vertex, outcoming_vertex, resistance, emf] = parser.get_current_edge_info();
    EXPECT_EQ(1, outcoming_vertex);
    EXPECT_EQ(2, incoming_vertex);
    EXPECT_EQ(std::complex<double>(0.1), resistance);
    EXPECT_TRUE(parser.is_emf_included());
    EXPECT_EQ(0.5, emf);
    EXPECT_TRUE(parser.is_eof());
}

TEST(SkipSyntaxTest, NoPunctuationTest) {
    input_parser parser("1 2 0.1 0.5V");
    parser.make_iteration();
    auto [incoming_vertex, outcoming_vertex, resistance, emf] = parser.get_current_edge_info();
    EXPECT_EQ(1, outcoming_vertex);
    EXPECT_EQ(2, incoming_vertex);
    std::complex<double>(0.1);
    EXPECT_EQ(std::complex<double>(0.1), resistance);
    EXPECT_TRUE(parser.is_emf_included());
    EXPECT_EQ(0.5, emf);
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

TEST(AcInputTest, SingleIterationNoEMFTest) {
    input_parser parser("1 -- 2, 0.1R; 0.2C; 0.3L; 1 -- 3, 0.4R; 0.5C; 0.6L;");
    parser.make_iteration();
    auto [incoming_vertex, outcoming_vertex, resistance, emf] = parser.get_current_edge_info();
    EXPECT_EQ(1, outcoming_vertex);
    EXPECT_EQ(2, incoming_vertex);
    std::complex<double>(0.1, 0.1);
    EXPECT_TRUE(std::complex<double>(0.1, 0.1) == resistance);
    EXPECT_FALSE(parser.is_emf_included());
    EXPECT_FALSE(parser.is_eof());
}

TEST(AcInputTest, SingleIterationWithEMFTest) {
    input_parser parser("1 -- 2, 0.1R; 0.2C; 0.3L; 0.5V, 5; 1 -- 3, 0.4R; 0.5C; 0.6L;");
    parser.make_iteration();
    auto [incoming_vertex, outcoming_vertex, resistance, emf] = parser.get_current_edge_info();
    EXPECT_EQ(1, outcoming_vertex);
    EXPECT_EQ(2, incoming_vertex);
    EXPECT_TRUE(std::complex<double>(0.1, 0.1) == resistance);
    EXPECT_TRUE(parser.is_emf_included());
    EXPECT_EQ(std::polar(0.5, -5.), emf);
    EXPECT_FALSE(parser.is_eof());
}

TEST(AcInputTest, TwoIterationWithEMFFirstNoEMFSecondTest) {
    input_parser parser("1 -- 2, 0.1R; 0.2C; 0.3L; 0.5V, 5; 1 -- 3, 0.4R; 0.5C; 0.6L;");
    parser.make_iteration();
    parser.make_iteration();
    auto [incoming_vertex, outcoming_vertex, resistance, emf] = parser.get_current_edge_info();
    EXPECT_EQ(1, outcoming_vertex);
    EXPECT_EQ(3, incoming_vertex);
    EXPECT_TRUE(std::complex<double>(0.4, 0.1) == resistance);
    EXPECT_FALSE(parser.is_emf_included());
    EXPECT_TRUE(parser.is_eof());
}

TEST(AcInputTest, TwoIterationNoEMFFirstWithEMFSecondTest) {
    input_parser parser("1 -- 2, 0.1R; 0.2C; 0.3L; 1 -- 3, 0.4R; 0.5C; 0.6L; 0.5V, 5;");
    parser.make_iteration();
    parser.make_iteration();
    auto [incoming_vertex, outcoming_vertex, resistance, emf] = parser.get_current_edge_info();
    EXPECT_EQ(1, outcoming_vertex);
    EXPECT_EQ(3, incoming_vertex);
    EXPECT_TRUE(std::complex<double>(0.4, 0.1) == resistance);
    EXPECT_TRUE(parser.is_emf_included());
    EXPECT_EQ(std::polar(0.5, -5.), emf);
    EXPECT_TRUE(parser.is_eof());
}

TEST(AcInputTest, TwoIterationWithEMFTest) {
    input_parser parser("1 -- 2, 0.1R; 0.2C; 0.3L; 0.5V, 5; 1 -- 3, 0.4R; 0.5C; 0.6L; 0.5V, 5;");
    parser.make_iteration();
    parser.make_iteration();
    auto [incoming_vertex, outcoming_vertex, resistance, emf] = parser.get_current_edge_info();
    EXPECT_EQ(1, outcoming_vertex);
    EXPECT_EQ(3, incoming_vertex);
    EXPECT_TRUE(std::complex<double>(0.4, 0.1) == resistance);
    EXPECT_TRUE(parser.is_emf_included());
    EXPECT_EQ(std::polar(0.5, -5.), emf);
    EXPECT_TRUE(parser.is_eof());
}

TEST(SkipSyntaxTest, NoSemicolonAfterAcResistanceTest) {
    input_parser parser("1 -- 2, 0.1R 0.2C; 0.3L; 0.5V, 5;");
    parser.make_iteration();
    auto [incoming_vertex, outcoming_vertex, resistance, emf] = parser.get_current_edge_info();
    EXPECT_EQ(1, outcoming_vertex);
    EXPECT_EQ(2, incoming_vertex);
    EXPECT_TRUE(std::complex<double>(0.1, 0.1) == resistance);
    EXPECT_TRUE(parser.is_emf_included());
    EXPECT_EQ(std::polar(0.5, -5.), emf);
    EXPECT_TRUE(parser.is_eof());
}

TEST(SkipSyntaxTest, NoSemicolonAfterAcCapacityTest) {
    input_parser parser("1 -- 2, 0.1R; 0.2C 0.3L; 0.5V, 5;");
    parser.make_iteration();
    auto [incoming_vertex, outcoming_vertex, resistance, emf] = parser.get_current_edge_info();
    EXPECT_EQ(1, outcoming_vertex);
    EXPECT_EQ(2, incoming_vertex);
    EXPECT_TRUE(std::complex<double>(0.1, 0.1) == resistance);
    EXPECT_TRUE(parser.is_emf_included());
    EXPECT_EQ(std::polar(0.5, -5.), emf);
    EXPECT_TRUE(parser.is_eof());
}

TEST(SkipSyntaxTest, NoSemicolonAfterAcInductanceTest) {
    input_parser parser("1 -- 2, 0.1R; 0.2C; 0.3L 0.5V, 5;");
    parser.make_iteration();
    auto [incoming_vertex, outcoming_vertex, resistance, emf] = parser.get_current_edge_info();
    EXPECT_EQ(1, outcoming_vertex);
    EXPECT_EQ(2, incoming_vertex);
    EXPECT_TRUE(std::complex<double>(0.1, 0.1) == resistance);
    EXPECT_TRUE(parser.is_emf_included());
    EXPECT_EQ(std::polar(0.5, -5.), emf);
    EXPECT_TRUE(parser.is_eof());
}

TEST(SkipSyntaxTest, NoSemicolonAfterAcEMFTest) {
    input_parser parser("1 -- 2, 0.1R; 0.2C; 0.3L; 0.5V, 5");
    parser.make_iteration();
    auto [incoming_vertex, outcoming_vertex, resistance, emf] = parser.get_current_edge_info();
    EXPECT_EQ(1, outcoming_vertex);
    EXPECT_EQ(2, incoming_vertex);
    EXPECT_TRUE(std::complex<double>(0.1, 0.1) == resistance);
    EXPECT_TRUE(parser.is_emf_included());
    EXPECT_EQ(std::polar(0.5, -5.), emf);
    EXPECT_TRUE(parser.is_eof());
}

TEST(SkipSyntaxTest, NoCommaAfterAcEMFAmplitudeTest) {
    input_parser parser("1 -- 2, 0.1R; 0.2C; 0.3L; 0.5V 5;");
    parser.make_iteration();
    auto [incoming_vertex, outcoming_vertex, resistance, emf] = parser.get_current_edge_info();
    EXPECT_EQ(1, outcoming_vertex);
    EXPECT_EQ(2, incoming_vertex);
    EXPECT_TRUE(std::complex<double>(0.1, 0.1) == resistance);
    EXPECT_TRUE(parser.is_emf_included());
    EXPECT_EQ(std::polar(0.5, -5.), emf);
    EXPECT_TRUE(parser.is_eof());
}

TEST(SkipSyntaxTest, NoPunctuationAcTest) {
    input_parser parser("1 2 0.1R 0.2C 0.3L 0.5V 5");
    parser.make_iteration();
    auto [incoming_vertex, outcoming_vertex, resistance, emf] = parser.get_current_edge_info();
    EXPECT_EQ(1, outcoming_vertex);
    EXPECT_EQ(2, incoming_vertex);
    EXPECT_TRUE(std::complex<double>(0.1, 0.1) == resistance);
    EXPECT_TRUE(parser.is_emf_included());
    EXPECT_EQ(std::polar(0.5, -5.), emf);
    EXPECT_TRUE(parser.is_eof());
}

TEST(ParserThrowsInvalidInputExceptionTest, UnexpectedCharacterAfterAcResistanceTest) {
    input_parser parser("1 -- 2, 0.1aR; 0.2C; 0.3L; 0.5V 5;");
    EXPECT_THROW(parser.make_iteration(), input_parser::InvalidInputException);
}

TEST(ParserThrowsInvalidInputExceptionTest, UnexpectedCharacterAfterAcResistanceDimensionTest) {
    input_parser parser("1 -- 2, 0.1Rf; 0.2C; 0.3L; 0.5V 5;");
    EXPECT_THROW(parser.make_iteration(), input_parser::InvalidInputException);
}

TEST(ParserThrowsInvalidInputExceptionTest, UnexpectedCharacterBeforeAcCapacityTest) {
    input_parser parser("1 -- 2, 0.1R;a 0.2C; 0.3L; 0.5V 5;");
    EXPECT_THROW(parser.make_iteration(), input_parser::InvalidInputException);
}

TEST(ParserThrowsInvalidInputExceptionTest, UnexpectedCharacterAfterAcCapacityTest) {
    input_parser parser("1 -- 2, 0.1R; 0.2aC; 0.3L; 0.5V 5;");
    EXPECT_THROW(parser.make_iteration(), input_parser::InvalidInputException);
}

TEST(ParserThrowsInvalidInputExceptionTest, UnexpectedCharacterAfterAcCapacityDimensionTest) {
    input_parser parser("1 -- 2, 0.1R; 0.2Ca; 0.3L; 0.5V 5;");
    EXPECT_THROW(parser.make_iteration(), input_parser::InvalidInputException);
}

TEST(ParserThrowsInvalidInputExceptionTest, UnexpectedCharacterBeforeAcInductanceTest) {
    input_parser parser("1 -- 2, 0.1R; 0.2C;a 0.3L; 0.5V 5;");
    EXPECT_THROW(parser.make_iteration(), input_parser::InvalidInputException);
}

TEST(ParserThrowsInvalidInputExceptionTest, UnexpectedCharacterAfterAcInductanceTest) {
    input_parser parser("1 -- 2, 0.1R; 0.2C; 0.3aL; 0.5V 5;");
    EXPECT_THROW(parser.make_iteration(), input_parser::InvalidInputException);
}

TEST(ParserThrowsInvalidInputExceptionTest, UnexpectedCharacterAfterAcInductanceDimensionTest) {
    input_parser parser("1 -- 2, 0.1R; 0.2C; 0.3La; 0.5V 5;");
    EXPECT_THROW(parser.make_iteration(), input_parser::InvalidInputException);
}

TEST(ParserThrowsInvalidInputExceptionTest, UnexpectedCharacterBeforeAcEMFTest) {
    input_parser parser("1 -- 2, 0.1R; 0.2C; 0.3L;a 0.5V 5;");
    EXPECT_THROW(parser.make_iteration(), input_parser::InvalidInputException);
}

TEST(ParserThrowsInvalidInputExceptionTest, UnexpectedCharacterAfterAcEMFAmplitudeTest) {
    input_parser parser("1 -- 2, 0.1R; 0.2C; 0.3L; 0.5aV 5;");
    parser.make_iteration();
    EXPECT_THROW(parser.make_iteration(), input_parser::InvalidInputException);
}

TEST(ParserThrowsInvalidInputExceptionTest, UnexpectedCharacterAfterAcEMFAmplitudeDimensionTest) {
    input_parser parser("1 -- 2, 0.1R; 0.2C; 0.3L; 0.5Va 5;");
    EXPECT_THROW(parser.make_iteration(), input_parser::InvalidInputException);
}

TEST(ParserThrowsInvalidInputExceptionTest, UnexpectedCharacterAfterAcEMFPhaseTest) {
    input_parser parser("1 -- 2, 0.1R; 0.2C; 0.3L; 0.5V 5a;");
    parser.make_iteration();
    EXPECT_THROW(parser.make_iteration(), input_parser::InvalidInputException);
}
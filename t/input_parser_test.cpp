#include <gtest/gtest.h>

#include "../input_parser.h"

TEST(ParseTest, SingleIterationNoEMFTest) {
    input_parser parser("1 -- 2, 0.1; 1 -- 3; 0.2;");
    parser.make_iteration();
    auto [incoming_vertex, outcoming_vertex, resistance, emf] = parser.get_current_edge_info();
    EXPECT_EQ(1, outcoming_vertex);
    EXPECT_EQ(2, incoming_vertex);
    EXPECT_DOUBLE_EQ(0.1, resistance);
    EXPECT_FALSE(parser.is_emf_included());
    EXPECT_FALSE(parser.is_eof());
}

TEST(ParseTest, SingleIterationWithEMFTest) {
    input_parser parser("1 -- 2, 0.1; 0.5V; 1 -- 3; 0.2;");
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
    input_parser parser("1 -- 2, 0.1; 0.5V; 1 -- 3; 0.2;");
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
    input_parser parser("1 -- 2, 0.1; 1 -- 3; 0.2; 0.6V;");
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
    input_parser parser("1 -- 2, 0.1; 0.5V; 1 -- 3; 0.2; 0.6V;");
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
#include <gtest/gtest.h>

#include "../graph.h"

TEST(EmptyGraphTest, EmptyGraphTest) {
    graph test_graph;
    EXPECT_EQ(0, test_graph.get_edge_number());
    EXPECT_EQ(0, test_graph.get_vertex_number());
}

TEST(AddEdgeTest, OneEdgeTest) {
    graph test_graph;
    test_graph.add_edge(0, 1);
    EXPECT_EQ(1, test_graph.get_edge_number());
    EXPECT_EQ(2, test_graph.get_vertex_number());
    EXPECT_EQ(0, test_graph.get_incoming_edges(1)[0]);
    EXPECT_EQ(0, test_graph.get_outcoming_edges(0)[0]);
    auto expected_vertices = std::pair<graph::vertex, graph::vertex>(0, 1);
    EXPECT_EQ(expected_vertices, test_graph.get_tied_vertices(0));
    EXPECT_NO_THROW(test_graph.check_connectivity());
}

TEST(AddEdgeTest, TwoEdgeTest) {
    graph test_graph;
    test_graph.add_edge(0, 1);
    test_graph.add_edge(1, 2);
    EXPECT_EQ(2, test_graph.get_edge_number());
    EXPECT_EQ(3, test_graph.get_vertex_number());
    EXPECT_EQ(0, test_graph.get_incoming_edges(1)[0]);
    EXPECT_EQ(1, test_graph.get_incoming_edges(2)[0]);
    EXPECT_EQ(0, test_graph.get_outcoming_edges(0)[0]);
    EXPECT_EQ(1, test_graph.get_outcoming_edges(1)[0]);
    auto expected_vertices = std::pair<graph::vertex, graph::vertex>(0, 1);
    EXPECT_EQ(expected_vertices, test_graph.get_tied_vertices(0));
    expected_vertices = std::pair<graph::vertex, graph::vertex>(1, 2);
    EXPECT_EQ(expected_vertices, test_graph.get_tied_vertices(1));
    EXPECT_NO_THROW(test_graph.check_connectivity());
}

TEST(AddEdgeTest, ThreeEdgeTest) {
    graph test_graph;
    test_graph.add_edge(0, 1);
    test_graph.add_edge(1, 2);
    test_graph.add_edge(0, 2);
    EXPECT_EQ(3, test_graph.get_edge_number());
    EXPECT_EQ(3, test_graph.get_vertex_number());
    EXPECT_EQ(0, test_graph.get_incoming_edges(1)[0]);
    EXPECT_EQ(1, test_graph.get_incoming_edges(2)[0]);
    EXPECT_EQ(2, test_graph.get_incoming_edges(2)[1]);
    EXPECT_EQ(0, test_graph.get_outcoming_edges(0)[0]);
    EXPECT_EQ(2, test_graph.get_outcoming_edges(0)[1]);
    EXPECT_EQ(1, test_graph.get_outcoming_edges(1)[0]);
    auto expected_vertices = std::pair<graph::vertex, graph::vertex>(0, 1);
    EXPECT_EQ(expected_vertices, test_graph.get_tied_vertices(0));
    expected_vertices = std::pair<graph::vertex, graph::vertex>(1, 2);
    EXPECT_EQ(expected_vertices, test_graph.get_tied_vertices(1));
    expected_vertices = std::pair<graph::vertex, graph::vertex>(0, 2);
    EXPECT_EQ(expected_vertices, test_graph.get_tied_vertices(2));
    EXPECT_NO_THROW(test_graph.check_connectivity());
}

TEST(GraphConnectivityTest, IncoherentSmallGraphTest) {
    graph test_graph;
    test_graph.add_edge(0, 0);
    test_graph.add_edge(1, 1);
    EXPECT_THROW(test_graph.check_connectivity(),graph::IncoherentGraphException);
}

TEST(GraphConnectivityTest, IncoherentGraphTest) {
    graph test_graph;
    test_graph.add_edge(0, 1);
    test_graph.add_edge(1, 0);
    test_graph.add_edge(2, 3);
    test_graph.add_edge(3, 2);
    EXPECT_THROW(test_graph.check_connectivity(),graph::IncoherentGraphException);
}

TEST(GraphConnectivityTest, CoherentGraphTest) {
    graph test_graph;
    test_graph.add_edge(0, 0);
    test_graph.add_edge(1, 1);
    test_graph.add_edge(1, 0);
    EXPECT_NO_THROW(test_graph.check_connectivity());
}

TEST(GraphConnectivityTest, CoherentSmallGraphTest) {
    graph test_graph;
    test_graph.add_edge(0, 0);
    EXPECT_NO_THROW(test_graph.check_connectivity());
}
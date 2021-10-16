#include "graph.h"

void graph::add_edge(vertex first, vertex second) {
    size_t max_vertex = std::max(first, second);
    if (max_vertex > incoming_edges.size()) {
        incoming_edges.resize(max_vertex);
        outcoming_edges.resize(max_vertex);
    }
    edge current = edges_vertices.size();
    size_t first_index = first - 1;
    size_t second_index = second - 1;
    incoming_edges[first_index].push_back(current);
    outcoming_edges[second_index].push_back(current);
    edges_vertices.emplace_back(first_index, second_index);
}
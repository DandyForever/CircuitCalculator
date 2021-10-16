#include "graph.h"

void graph::add_edge(vertex first, vertex second) {
    size_t max_vertex = std::max(first + 1, second + 1);
    resize_structures(max_vertex);
    edge current = edges_vertices.size();
    outcoming_edges[first].push_back(current);
    incoming_edges[second].push_back(current);
    edges_vertices.emplace_back(first, second);
}

void graph::resize_structures(size_t necessary_size) {
    if (necessary_size > incoming_edges.size()) {
        incoming_edges.resize(necessary_size);
        outcoming_edges.resize(necessary_size);
    }
}
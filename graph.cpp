#include "graph.h"

bool graph::add_edge(vertex first, vertex second) {
    size_t max_vertex = std::max(first + 1, second + 1);
    resize_structures(max_vertex);
    if (first == second) {
        loops.push_back(first);
        return false;
    }
    edge current = edges_vertices.size();
    outcoming_edges[first].push_back(current);
    incoming_edges[second].push_back(current);
    edges_vertices.emplace_back(first, second);
    return true;
}

void graph::resize_structures(size_t necessary_size) {
    if (necessary_size > incoming_edges.size()) {
        incoming_edges.resize(necessary_size);
        outcoming_edges.resize(necessary_size);
    }
}

void graph::check_connectivity() const {
    std::set<vertex> isolated_vertices;
    dfs(isolated_vertices, 0);
    if (isolated_vertices.size() != incoming_edges.size()) {
        print_isolated_vertices(isolated_vertices);
        throw IncoherentGraphException();
    }
}

void graph::print_isolated_vertices(const std::set<vertex> &isolated_vertices) const {
    std::cout << "Isolated vertices: ";
    for (auto vertex_ : isolated_vertices)
        std::cout << vertex_ + 1 << " ";
    std::cout << std::endl;
}

void graph::dfs(std::set<vertex>& visited_vertices, vertex current_vertex) const {
    visited_vertices.insert(current_vertex);
    for (auto edge_ : incoming_edges[current_vertex]) {
        auto [source, destination] = edges_vertices[edge_];
        if (!visited_vertices.count(source))
            dfs(visited_vertices, source);
    }
    for (auto edge_ : outcoming_edges[current_vertex]) {
        auto [source, destination] = edges_vertices[edge_];
        if (!visited_vertices.count(destination))
            dfs(visited_vertices, destination);
    }
}
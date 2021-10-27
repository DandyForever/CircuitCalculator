#include <iterator>
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

std::vector<graph> graph::get_coherent_subgraphs() const {
    std::vector<graph> subgraphs;
    std::set<vertex> all_vertices = get_all_vertices_set();
    while (!all_vertices.empty()) {
        std::set<vertex> isolated_vertices;
        dfs(isolated_vertices, *all_vertices.begin());
        modify_all_vertices_set(all_vertices, isolated_vertices);
        fill_subgraph_vertices_mapping(isolated_vertices);
        auto subgraph = fill_subgraph(subgraphs.size(), isolated_vertices);
        subgraphs.push_back(subgraph);
    }
    return subgraphs;
}

void graph::modify_all_vertices_set(std::set<vertex> &all_vertices, const std::set<vertex> &isolated_vertices) const {
    std::set<vertex> unvisited_vertices;
    std::set_difference(all_vertices.begin(), all_vertices.end(),
                        isolated_vertices.begin(), isolated_vertices.end(),
                        std::inserter(unvisited_vertices, unvisited_vertices.end()));
    all_vertices = std::move(unvisited_vertices);
}

std::set<graph::vertex> graph::get_all_vertices_set() const {
    std::set<vertex> all_vertices;
    for (size_t vertex_index = 0; vertex_index < outcoming_edges.size(); vertex_index++) {
        all_vertices.insert(vertex_index);
    }
    return all_vertices;
}

graph graph::fill_subgraph(size_t subgraph_index, std::set<vertex> &isolated_vertices) const {
    graph subgraph;
    size_t subgraph_edge_index = 0;
    for (auto vertex_ : isolated_vertices) {
        for (auto edge_index : outcoming_edges[vertex_]) {
            auto edge = edges_vertices[edge_index];
            subgraph.add_edge(subgraph_vertices_mapping[edge.first],
                              subgraph_vertices_mapping[edge.second]);
            subgraph_edges_mapping[{subgraph_index, subgraph_edge_index++}] = edge_index;
        }
    }
    return subgraph;
}

void graph::fill_subgraph_vertices_mapping(std::set<vertex> &isolated_vertices) const {
    vertex subgraph_vertex = 0;
    for (auto vertex_ : isolated_vertices) {
        subgraph_vertices_mapping[vertex_] = subgraph_vertex++;
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
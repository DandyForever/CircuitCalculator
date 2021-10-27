#ifndef CIRCUITS_GRAPH_H
#define CIRCUITS_GRAPH_H

#include <vector>
#include <set>
#include <iostream>
#include <algorithm>
#include <map>

class graph {
public:
    typedef size_t edge;
    typedef size_t vertex;
    typedef std::vector<edge> edges;
    typedef std::pair<vertex, vertex> tied_vertices;
    typedef std::pair<size_t, edge> subgraph_mapping_key;

    bool add_edge(vertex first, vertex second);
    size_t get_edge_number() const { return edges_vertices.size(); }
    size_t get_vertex_number() const { return incoming_edges.size(); }
    const edges& get_incoming_edges(vertex vertex_) const { return incoming_edges[vertex_]; }
    const edges& get_outcoming_edges(vertex vertex_) const { return outcoming_edges[vertex_]; }
    const tied_vertices& get_tied_vertices(edge edge_) const { return edges_vertices[edge_]; }
    edge get_subgraph_mapping_edge(subgraph_mapping_key key) const { return subgraph_edges_mapping[key]; }
    const std::vector<vertex>& get_loops() const { return loops; }

    void check_connectivity() const;
    std::vector<graph> get_coherent_subgraphs() const;
    void dfs(std::set<vertex>& visited_vertices, vertex current_vertex) const;
    class IncoherentGraphException : public std::exception {
        const char* what () const throw () { return "Circuit graph is incoherent!"; }
    };
private:
    std::vector<edges> incoming_edges;
    std::vector<edges> outcoming_edges;
    std::vector<tied_vertices> edges_vertices;
    std::vector<vertex> loops;

    mutable std::map<subgraph_mapping_key, edge> subgraph_edges_mapping;
    mutable std::map<vertex, vertex> subgraph_vertices_mapping;

    void resize_structures(size_t necessary_size);
    void print_isolated_vertices(const std::set<vertex> &isolated_vertices) const;

    void fill_subgraph_vertices_mapping(std::set<vertex> &isolated_vertices) const;

    graph fill_subgraph(size_t subgraph_index, std::set<vertex> &isolated_vertices) const;

    std::set<vertex> get_all_vertices_set() const;

    void modify_all_vertices_set(std::set<vertex> &all_vertices, const std::set<vertex> &isolated_vertices) const;
};


#endif //CIRCUITS_GRAPH_H

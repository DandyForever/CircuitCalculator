#ifndef CIRCUITS_GRAPH_H
#define CIRCUITS_GRAPH_H

#include <vector>
#include <set>
#include <iostream>

class graph {
public:
    typedef size_t edge;
    typedef size_t vertex;
    typedef std::vector<edge> edges;
    typedef std::pair<vertex, vertex> tied_vertices;

    bool add_edge(vertex first, vertex second);
    size_t get_edge_number() const { return edges_vertices.size(); }
    size_t get_vertex_number() const { return incoming_edges.size(); }
    const edges& get_incoming_edges(vertex vertex_) const { return incoming_edges[vertex_]; }
    const edges& get_outcoming_edges(vertex vertex_) const { return outcoming_edges[vertex_]; }
    const tied_vertices& get_tied_vertices(edge edge_) const { return edges_vertices[edge_]; }
    const std::vector<vertex>& get_loops() const { return loops; }

    void check_connectivity() const;
    void dfs(std::set<vertex>& visited_vertices, vertex current_vertex) const;
    class IncoherentGraphException : public std::exception {
        const char* what () const throw () { return "Circuit graph is incoherent!"; }
    };
private:
    std::vector<edges> incoming_edges;
    std::vector<edges> outcoming_edges;
    std::vector<tied_vertices> edges_vertices;
    std::vector<vertex> loops;

    void resize_structures(size_t necessary_size);
    void print_isolated_vertices(const std::set<vertex> &isolated_vertices) const;
};


#endif //CIRCUITS_GRAPH_H
